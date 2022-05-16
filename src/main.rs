mod fw;

use fw::core::safequeue::SafeQueue;
use fw::core::sockmsg::SockMsg;
use fw::model::character::{ Character };

use std::collections::HashMap;
use std::net::{TcpListener, TcpStream, Shutdown, SocketAddr};
use std::io::{ErrorKind, Read, Write};
use std::thread::{JoinHandle, sleep};
use std::time::Duration;
use std::sync::mpsc::{Sender, Receiver, TryRecvError};
use std::sync::mpsc;
use std::sync::Arc;
use std::thread;

fn handle_client_recv(mut stream: TcpStream, fd: usize, chan_recv: Arc<SafeQueue<SockMsg>>) {
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
                msg: String::from_utf8(Vec::from(&data[0..last_size])).unwrap().replace("\r\n", "").replace("\n", "")
            };

            println!("Message from client #{}: {}", tmp.fd, tmp.msg);
            chan_recv.push_back(tmp);
        } else if last_size == 0 {
            stream.shutdown(Shutdown::Both).unwrap();

            chan_recv.push_back(SockMsg {
                fd,
                msg: String::new()
            });
        }
    }
}

fn handle_client_send(mut stream: TcpStream, chan_send: Arc<SafeQueue<SockMsg>>) {
    loop {
        while chan_send.len() > 0 {
            let tmp = chan_send.pop_front();

            println!("Sending to #{}: {}", tmp.fd, tmp.msg);
            stream.write(tmp.msg.as_ref()).unwrap();
        }

        sleep(Duration::new(0, 5000000));
    }
}

fn main() {
    let (listen_send, listen_recv): (Sender<TcpStream>, Receiver<TcpStream>) = mpsc::channel();
    let mut chars: Vec<Character>        = Vec::new();
    let mut threads: Vec<JoinHandle<()>> = Vec::new();
    let mut conns: HashMap<usize, usize> = HashMap::new();

    let listener = TcpListener::bind("0.0.0.0:6055").unwrap();
    println!("Server listening on port 6055");

    let listener_thread = thread::spawn(move || {
        for stream in listener.incoming() {
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
        for stream_recv in listen_recv.try_recv() {
            println!("New connection from {}", stream_recv.peer_addr().unwrap());

            let new_fd = chars.len() + 1;
            let stream_send = stream_recv.try_clone().unwrap();
            chars.push(Character::new_from_vnum(new_fd.clone() as u32));

            let chan_recv = Arc::clone(&chars[chars.len() - 1].chan_recv);
            let chan_send = Arc::clone(&chars[chars.len() - 1].chan_send);

            threads.push(thread::spawn(move || {
                handle_client_recv(stream_recv, new_fd, chan_recv);
            }));

            threads.push(thread::spawn(move || {
                handle_client_send(stream_send, chan_send);
            }));
        }

        for ch in &chars {
            while ch.chan_recv.len() > 0 {
                let msg = ch.chan_recv.pop_front();

                println!("#{}: {}", msg.fd, msg.msg);
                ch.chan_send.push_back(SockMsg {
                    fd: ch.vnum as usize,
                    msg: format!("You sent: {}\n", msg.msg),
                })
            }
        }

        sleep(Duration::new(0, 5000000));
    }

    // close the socket server
    drop(listener);
}
