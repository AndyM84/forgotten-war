mod fw;

use fw::core::sockmsg::SockMsg;
use fw::model::character::{ Character };

use std::collections::HashMap;
use std::net::{TcpListener, TcpStream, Shutdown, SocketAddr};
use std::io::{ErrorKind, Read, Write};
use std::thread::{JoinHandle, sleep};
use std::time::Duration;
use std::sync::mpsc::{Sender, Receiver, TryRecvError};
use std::sync::mpsc;
use std::thread;

fn handle_client(mut stream: TcpStream, fd: usize, chan: Sender<SockMsg>) {
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
            chan.send(SockMsg {
                fd,
                msg: String::from_utf8(Vec::from(&data[0..last_size])).unwrap().replace("\r\n", "").replace("\n", "")
            }).unwrap();
        } else if last_size == 0 {
            stream.shutdown(Shutdown::Both).unwrap();

            chan.send(SockMsg {
                fd,
                msg: String::new()
            }).unwrap();
        }
    }
}

fn main() {
    // NOTE:
    //  - Need to look into Arc<T> to see if we can use pointers to share a message queue instead
    //    of these damn channels.

    let (listen_send, listen_recv): (Sender<TcpStream>, Receiver<TcpStream>) = mpsc::channel();
    let (chan_sender, chan_recvr): (Sender<SockMsg>, Receiver<SockMsg>) = mpsc::channel();
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
        for stream in listen_recv.try_recv() {
            println!("New connection from {}", stream.peer_addr().unwrap());

            let new_fd = chars.len() + 1;
            chars.push(Character::new_from_vnum(new_fd.clone() as u32));
            let chan_send = chan_sender.clone();

            threads.push(thread::spawn(move || {
                handle_client(stream, new_fd, chan_send);
            }));
        }

        for msg in chan_recvr.try_iter() {
            if msg.msg.len() < 1 {
                println!("Client #{} disconnected, cleaning them up", msg.fd);
            } else {
                println!("{}:{}", msg.fd, msg.msg);
            }
        }

        sleep(Duration::new(0, 5000000));
    }

    // close the socket server
    drop(listener);
}
