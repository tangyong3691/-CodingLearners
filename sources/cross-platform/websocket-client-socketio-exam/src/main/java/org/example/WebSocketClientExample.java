package org.example;

import io.socket.client.IO;
import io.socket.client.Socket;
import io.socket.emitter.Emitter;

import java.util.Scanner;
import java.util.concurrent.CompletableFuture;


public class WebSocketClientExample {
    private static Socket socket;
    private static CompletableFuture<Void> connectFuture;
    private static CompletableFuture<Void> disconnectFuture;
    private static CompletableFuture<Void> receiveFuture;

    public static void main(String[] args) {
        try {
            connectFuture = new CompletableFuture<>();
            disconnectFuture = new CompletableFuture<>();
            receiveFuture = new CompletableFuture<>();
            System.out.println("start ty....");
            // Connect to the Socket.IO server
            socket = IO.socket("http://127.0.0.1:3009");

            // Connect to the server
            socket.on(Socket.EVENT_CONNECT, new Emitter.Listener() {
                @Override
                public void call(Object... args) {
                    System.out.println("Connected to the server");
                    connectFuture.complete(null);
                }
            });
            socket.connect();

            // Handle disconnection and reconnect
            socket.on(Socket.EVENT_DISCONNECT, new Emitter.Listener() {
                @Override
                public void call(Object... args) {
                    System.out.println("Disconnected from the server");
                    disconnectFuture.complete(null);
                    // Reconnect after a short delay
                    try {
                        Thread.sleep(2000);
                        socket.connect();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            });

            // Receive a message from the server
            socket.on("chat message", new Emitter.Listener() {
                @Override
                public void call(Object... args) {
                    String message = (String) args[0];
                    System.out.println("Received message: " + message);
                    receiveFuture.complete(null);
                }
            });

            // Send and receive messages asynchronously
            connectFuture.thenRunAsync(new Runnable() {
                @Override
                public void run() {
                    while (true) {
                        Scanner scanner = new Scanner(System.in);
                        System.out.println("Enter a message to send (or 'exit' to quit):");
                        String input = scanner.nextLine();
                        if (input.equalsIgnoreCase("exit")) {
                            // Disconnect and exit the program
                            socket.disconnect();
                            break;
                        } else {
                            // Send the message to the server
                            socket.emit("chat message", input);
                            System.out.println("Message sent: " + input);

                        }
                    }
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}