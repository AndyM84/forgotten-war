mod fw;

use fw::core::mudmsg::{MudMsg, MudMsgTypes};
use fw::core::safequeue::SafeQueue;
use fw::core::sockmsg::{SockMsg, SockMsgStates};
use fw::core::FW::FW;
use fw::model::character::{Character};

use std::collections::HashMap;
use std::fmt::format;
use std::net::{TcpListener, TcpStream, Shutdown};
use std::io::{ErrorKind, Read, Write};
use std::thread::sleep;
use std::time::Duration;
use std::sync::mpsc::{Sender, Receiver};
use std::sync::mpsc;
use std::sync::Arc;
use std::thread;
use crate::fw::core::connhandler::ConnHandler;
use crate::fw::model::enums;

extern crate signal_hook;

static mut KEEP_RUNNING: bool = true;

pub fn reg_for_sigs() {
    unsafe { signal_hook::register(signal_hook::SIGINT, || on_sigint()) }
      .and_then(|_| {
          println!("Registered for SIGINT");
          Ok(())
      })
      .or_else(|e| {
          println!("Failed to register for SIGINT {:?}", e);
          Err(e)
      })
      .ok();
}

fn on_sigint() {
    println!("SIGINT caught - exiting");
    println!();

    unsafe {
        KEEP_RUNNING = false;
    }
}

fn handle_client_recv(mut stream: TcpStream, fd: u32, chan_recv: Arc<SafeQueue<SockMsg>>) {
    let mut data = [0 as u8; 50]; // using 50 byte buffer
    let mut last_size = 0 as usize;

    while match stream.read(&mut data) {
        Ok(size) => {
            last_size = size;
            true
        },
        Err(err) => {
            let os_err = err.raw_os_error().unwrap();

            if os_err != 10053 && os_err != 10093 {
                println!("An error occurred, terminating connection with {}", stream.peer_addr().unwrap());

                stream.shutdown(Shutdown::Both).expect("failed to shut down stream");
            }

            false
        }
    } {
        if last_size > 0 {
            let tmp = SockMsg {
                fd: fd.clone(),
                msg: String::from_utf8(Vec::from(&data[0..last_size])).unwrap().replace("\r\n", "").replace("\n", ""),
                state: SockMsgStates::Active
            };

            println!("Message from client #{}: {}", tmp.fd.clone(), tmp.msg);
            chan_recv.push_back(tmp);
        } else if last_size == 0 {
            stream.shutdown(Shutdown::Both).unwrap();

            chan_recv.push_back(SockMsg {
                fd: fd.clone(),
                msg: String::new(),
                state: SockMsgStates::Disconnect
            });

            break;
        }
    }
}

fn handle_client_send(mut stream: TcpStream, chan_send: Arc<SafeQueue<SockMsg>>) {
    'outer: loop {
        while chan_send.len() > 0 {
            let tmp = chan_send.pop_front();

            if tmp.state == SockMsgStates::Disconnect {
                stream.shutdown(Shutdown::Both).expect("error shutting down stream");

                break 'outer;
            }

            unsafe {
                if KEEP_RUNNING.clone() {
                    println!("Sending: {}", tmp.msg);
                }
            }

            stream.write(tmp.msg.as_ref()).expect("stream closed before client send could complete");
        }

        sleep(Duration::new(0, 5000000));
    }
}

fn main() {
    let mut fw: FW = FW::new();
    let (listen_send, listen_recv): (Sender<TcpStream>, Receiver<TcpStream>) = mpsc::channel();

    reg_for_sigs();

    let listener = TcpListener::bind("0.0.0.0:6055").unwrap();
    let listener_copy = listener.try_clone().unwrap();

    println!("Server listening on port 6055");
    println!();
    println!("Forgotten War has begun!");
    println!();

    thread::spawn(move || {
        for stream in listener_copy.incoming() {
            match stream {
                Ok(stream) => {
                    listen_send.send(stream).unwrap();
                }
                Err(e) if e.kind() == ErrorKind::WouldBlock => {
                    continue;
                }
                Err(e) => {
                    // Useless WSAStartup error on Windows during shutdown
                    if e.raw_os_error().unwrap() == 10004 {
                        break;
                    }

                    println!("Error: {}", e);

                    break;
                }
            }
        }
    });

    loop {
        for stream_recv in listen_recv.try_recv() {
            fw.char_index += 1;
            let new_vnum = fw.char_index.clone();

            fw.conn_index += 1;
            let new_conn = fw.conn_index.clone();

            println!("New connection from {}, conn {} vnum {}",
                     stream_recv.peer_addr().unwrap(),
                     new_conn,
                     new_vnum);

            let stream_send = stream_recv.try_clone().unwrap();
            let stream_copy = stream_recv.try_clone().unwrap();
            let mut char = Character::new_from_vnum(new_vnum.clone());
            char.connection_state = enums::ConnectionStates::Connected;

            let chan_recv = Arc::clone(&char.chan_recv);
            let chan_send = Arc::clone(&char.chan_send);

             let recv_thread = thread::spawn(move || {
                handle_client_recv(stream_recv, new_vnum, chan_recv);
            });

            let send_thread = thread::spawn(move || {
                handle_client_send(stream_send, chan_send);
            });

            fw.chars.insert(new_vnum.clone(), char);
            fw.conns.insert(new_conn.clone(), ConnHandler::new(new_vnum.clone(), recv_thread, send_thread, stream_copy));

            fw.char_conn.insert(new_vnum.clone(), new_conn.clone());
            fw.conn_char.insert(new_conn, new_vnum);
        }

        let (messages, disconnects) = fw.process_tick();

        for ch in &disconnects {
            fw.chars.remove(ch);
            fw.conns.remove(ch);
        }

        for msg in messages {
            // for (vnum, ch) in &fw.chars {
            //     ch.chan_send.push_back(SockMsg {
            //         fd: fw.char_conn[vnum].clone(),
            //         msg: format!("{}\n", msg.msg_contents.clone()),
            //         state: SockMsgStates::Active
            //     });
            // }


        }

        unsafe {
            if !KEEP_RUNNING.clone() {
                break;
            }
        }

        sleep(Duration::new(0, 5000000));
    }

    println!("Beginning shutdown process..");
    sleep(Duration::new(1, 50000000));

    print!("  Disconnecting any active clients..");

    for (vnum, ch) in &fw.chars {
        ch.chan_send.push_back(SockMsg {
            fd: fw.char_conn[vnum].clone(),
            msg: String::from("The server is shutting down. Until next time...\n\n"),
            state: SockMsgStates::Active
        });
    }

    sleep(Duration::new(1, 50000000));

    for (vnum, ch) in &fw.chars {
        ch.chan_send.push_back(SockMsg {
            fd: fw.char_conn[vnum].clone(),
            msg: String::new(),
            state: SockMsgStates::Disconnect
        });
    }

    println!(" DONE");

    print!("  Closing listener socket..");
    drop(listener);
    println!(" DONE");
    println!("Shutdown complete.");

    println!();
    println!("Thanks for hosting!");
}
