mod fw;

use fw::core::safequeue::SafeQueue;
use fw::core::sockmsg::{SockMsg, SockMsgStates};
use fw::model::character::{Character};

use lazy_static::lazy_static;
use std::collections::HashMap;
use std::net::{TcpListener, TcpStream, Shutdown, SocketAddr};
use std::io::{ErrorKind, Read, Write};
use std::ops::Deref;
use std::thread::{JoinHandle, sleep};
use std::time::Duration;
use std::sync::mpsc::{Sender, Receiver, TryRecvError};
use std::sync::mpsc;
use std::sync::Arc;
use std::thread;
use crate::fw::core::connhandler::ConnHandler;

extern crate signal_hook;

static mut KEEP_RUNNING: bool = true;
//static mut KEEP_RUNNING_2: &'static Arc<bool> = || &Arc::new(false);

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
            println!("An error occurred, terminating connection with {}", stream.peer_addr().unwrap());
            println!("{}", err);

            stream.shutdown(Shutdown::Both).unwrap();

            false
        }
    } {
        if last_size > 0 {
            let tmp = SockMsg {
                fd,
                msg: String::from_utf8(Vec::from(&data[0..last_size])).unwrap().replace("\r\n", "").replace("\n", ""),
                state: SockMsgStates::Active
            };

            println!("Message from client #{}: {}", tmp.fd, tmp.msg);
            chan_recv.push_back(tmp);
        } else if last_size == 0 {
            stream.shutdown(Shutdown::Both).unwrap();

            chan_recv.push_back(SockMsg {
                fd,
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
                stream.shutdown(Shutdown::Both).unwrap();

                break 'outer;
            }

            println!("Sending: {}", tmp.msg);
            stream.write(tmp.msg.as_ref()).unwrap();
        }

        sleep(Duration::new(0, 5000000));
    }
}

fn main() {
    let (listen_send, listen_recv): (Sender<TcpStream>, Receiver<TcpStream>) = mpsc::channel();
    let mut char_index: u32 = 0;
    let mut chars: HashMap<u32, Character> = HashMap::new();
    let mut conns: HashMap<u32, ConnHandler> = HashMap::new();

    let listener = TcpListener::bind("0.0.0.0:6055").unwrap();
    let listener_copy = listener.try_clone().unwrap();
    println!("Server listening on port 6055");

    reg_for_sigs();

    let listener_thread = thread::spawn(move || {
        for stream in listener_copy.incoming() {
            match stream {
                Ok(stream) => {
                    listen_send.send(stream).unwrap();
                }
                Err(e) if e.kind() == ErrorKind::WouldBlock => {
                    continue;
                }
                Err(e) => {
                    println!("Error: {}", e);
                }
            }
        }
    });

    loop {
        let mut disconnected: Vec<u32> = Vec::new();

        for stream_recv in listen_recv.try_recv() {
            println!("New connection from {}", stream_recv.peer_addr().unwrap());

            let new_fd = char_index + 1;
            char_index += 1;

            let stream_send = stream_recv.try_clone().unwrap();
            let stream_copy = stream_recv.try_clone().unwrap();
            let mut char = Character::new_from_vnum(new_fd.clone());

            let chan_recv = Arc::clone(&char.chan_recv);
            let chan_send = Arc::clone(&char.chan_send);

             let mut recv_thread = thread::spawn(move || {
                handle_client_recv(stream_recv, new_fd, chan_recv);
            });

            let mut send_thread = thread::spawn(move || {
                handle_client_send(stream_send, chan_send);
            });

            chars.insert(new_fd.clone(), char);
            conns.insert(new_fd.clone(), ConnHandler::new(new_fd.clone(), recv_thread, send_thread, stream_copy));
        }

        for (idx, ch) in &chars {
            while ch.chan_recv.len() > 0 {
                let msg = ch.chan_recv.pop_front();

                if !conns.contains_key(&ch.vnum) {
                    disconnected.push(ch.vnum);

                    continue;
                }

                if msg.msg.len() == 0 {
                    conns[&ch.vnum].shutdown();
                    disconnected.push(ch.vnum);
                    println!("Connect #{} was disconnected", ch.vnum);

                    continue;
                }

                println!("#{}: {}", msg.fd, msg.msg);
                ch.chan_send.push_back(SockMsg {
                    fd: ch.vnum,
                    msg: format!("You sent: {}\n", msg.msg),
                    state: SockMsgStates::Active
                });
            }
        }

        for ch in &disconnected {
            chars.remove(ch);
            conns.remove(ch);
        }

        unsafe {
            if !KEEP_RUNNING {
                break;
            }

            // if (*KEEP_RUNNING_2) == Arc::from(false) {
            //     break;
            // }
        }

        sleep(Duration::new(0, 5000000));
    }

    // close the socket server
    drop(&listener);
}
