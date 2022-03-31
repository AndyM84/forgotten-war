mod fw;

use fw::model::character::{ Character };

use std::net::{TcpListener, TcpStream, Shutdown, SocketAddr};
use std::io::{ErrorKind, Read, Write};
use std::thread::{JoinHandle, sleep};
use std::time::Duration;
use std::sync::mpsc::{Sender, Receiver};
use std::sync::mpsc;
use std::thread;

struct SockMsg {
    fd: SocketAddr,
    msg: String
}

fn handle_client(mut stream: TcpStream, chan: Sender<SockMsg>) {
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
                fd: stream.peer_addr().unwrap(),
                msg: String::from_utf8(Vec::from(&data[0..last_size])).unwrap()
            }).unwrap();
        }
    }
}

fn main() {
    let (chan_sender, chan_recvr): (Sender<SockMsg>, Receiver<SockMsg>) = mpsc::channel();
    let mut chars: Vec<Character>        = Vec::new();
    let mut threads: Vec<JoinHandle<()>> = Vec::new();

    let listener = TcpListener::bind("0.0.0.0:6055").unwrap();
    println!("Server listening on port 6055");

    let listener_thread = thread::spawn(move || {
        for stream in listener.incoming() {
            match stream {
                Ok(stream) => {
                    println!("New connection: {}", stream.peer_addr().unwrap());

                    let chan_send = chan_sender.clone();

                    threads.push(thread::spawn(move || {
                        handle_client(stream, chan_send);
                    }));
                }
                Err(e) if e.kind() == ErrorKind::WouldBlock => {
                    continue;
                }
                Err(e) => {
                    //println!("Error: {}", e);
                    /* connection failed */
                }
            }
        }
    });

    loop {
        for _ in 0..chars.len() {
            if let msg = chan_recvr.recv_timeout(Duration::new(0, 5000000)).unwrap() {
                println!("{}:{}", msg.fd, msg.msg);
            }
        }

        sleep(Duration::new(0, 5000000));
    }

    // close the socket server
    drop(listener);
}
