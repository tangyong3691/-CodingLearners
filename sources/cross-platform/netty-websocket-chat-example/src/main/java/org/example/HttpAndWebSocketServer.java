package org.example;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.util.CharsetUtil;
import io.netty.util.AttributeKey;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.http.*;
import io.netty.handler.ssl.SslContext;
import io.netty.handler.ssl.SslContextBuilder;
import io.netty.handler.ssl.util.SelfSignedCertificate;
import io.netty.handler.stream.ChunkedFile;
import io.netty.handler.stream.ChunkedWriteHandler;
import io.netty.handler.stream.ChunkedNioStream;

import io.netty.buffer.Unpooled;


import io.netty.handler.codec.http.websocketx.*;
import io.netty.handler.codec.http.websocketx.extensions.compression.WebSocketServerCompressionHandler;
import io.netty.handler.codec.http.websocketx.extensions.compression.WebSocketServerCompressionHandler;
import io.netty.handler.codec.http.websocketx.extensions.compression.WebSocketServerCompressionHandler;
import java.io.File;
import java.io.RandomAccessFile;
import java.util.Map;
import java.util.List;
import java.util.HashMap;
import java.net.URI;
import java.util.concurrent.TimeUnit;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.io.InputStream;
import java.io.IOException;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;

import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.Inet4Address;
import java.net.InetAddress;

import java.net.NetworkInterface;
import java.util.Enumeration;

public class HttpAndWebSocketServer {
    private static final AttributeKey<Integer> KEEP_ALIVE_TIMEOUT = AttributeKey.newInstance("keep_alive_timeout");
    private static List<Channel> wbschannels = new ArrayList<>();
    private final int port;
    private final String staticFilesPath;
    //private static int websktshak = 0;

    public HttpAndWebSocketServer(int port, String staticFilesPath) {
        this.port = port;
        this.staticFilesPath = staticFilesPath;
    }

    public void start() throws Exception {
        // Configure SSL

        EventLoopGroup bossGroup = new NioEventLoopGroup();
        EventLoopGroup workerGroup = new NioEventLoopGroup();
        try {
            ServerBootstrap bootstrap = new ServerBootstrap();
            bootstrap.group(bossGroup, workerGroup)
                    .channel(NioServerSocketChannel.class)
                    .childHandler(new ChannelInitializer<SocketChannel>() {
                        @Override
                        protected void initChannel(SocketChannel ch) throws Exception {
                            ChannelPipeline pipeline = ch.pipeline();
                            pipeline.addLast(new HttpServerCodec());
                            pipeline.addLast(new HttpObjectAggregator(65536));
                            pipeline.addLast(new ChunkedWriteHandler());
                            pipeline.addLast(new WebSocketServerHandler());
                            pipeline.addLast(new HttpServerHandler());


                        }
                    });

            ChannelFuture future = bootstrap.bind(port).sync();
            Channel channel = future.channel();

            SocketAddress local = channel.localAddress();

            List<String> validAddresses = new ArrayList<>();

            if (local instanceof InetSocketAddress) {
                Enumeration<NetworkInterface> networkInterfaces = NetworkInterface.getNetworkInterfaces();
                while (networkInterfaces.hasMoreElements()) {
                    NetworkInterface networkInterface = networkInterfaces.nextElement();
                    Enumeration<InetAddress> inetAddresses = networkInterface.getInetAddresses();
                    while (inetAddresses.hasMoreElements()) {
                        InetAddress inetAddress = inetAddresses.nextElement();

                        if (inetAddress instanceof Inet4Address && !inetAddress.isLoopbackAddress() && !inetAddress.isLinkLocalAddress() && !inetAddress.isMulticastAddress()) {
                            validAddresses.add(inetAddress.getHostAddress());
                        }
                    }
                }
            }

            for (String address : validAddresses) {
                System.out.println("HTTP Server deployed at: http://" + address + ":" + port);
            }

            future.channel().closeFuture().sync();
        } finally {
            bossGroup.shutdownGracefully();
            workerGroup.shutdownGracefully();
        }
    }

    private class WebSocketServerHandler extends SimpleChannelInboundHandler<WebSocketFrame> {
        private WebSocketServerHandshaker handshaker;


        @Override
        protected void channelRead0(ChannelHandlerContext ctx, WebSocketFrame frame) throws Exception {
            // Handle WebSocket frame
            if (frame instanceof TextWebSocketFrame) {
                // Handle text frame
                TextWebSocketFrame textFrame = (TextWebSocketFrame) frame;
                String text = textFrame.text();
                // Process the received text message
                System.out.println("Received text: " + text);
                if(text.equals("2probe")){
                    TextWebSocketFrame response = new TextWebSocketFrame("3probe");
                    ctx.channel().writeAndFlush(response);
                }else {

                    Pattern pattern = Pattern.compile("\\d+\\[\"(.+)\",\"(.+)\"]");
                    Matcher matcher = pattern.matcher(text);
                    if(matcher.find()) {
                        String substring1 = matcher.group(1);
                        String substring2 = matcher.group(2);
                        //System.out.println(substring1 + "----" + substring2); // hello
                        if(substring1.equals("chat message")){
                            for(Channel channel : wbschannels) {
                                channel.writeAndFlush(new TextWebSocketFrame(text));
                            }
                        }

                    }

                }
            } else if (frame instanceof BinaryWebSocketFrame) {
                // Handle binary frame
                BinaryWebSocketFrame binaryFrame = (BinaryWebSocketFrame) frame;
                // Process the received binary message
                // ...
            } else if (frame instanceof PingWebSocketFrame) {
                // Handle ping frame
                PingWebSocketFrame pingFrame = (PingWebSocketFrame) frame;
                // Process the received ping message
                handlePingFrame(ctx, pingFrame);
            } else if (frame instanceof PongWebSocketFrame) {
                // Handle pong frame
                PongWebSocketFrame pongFrame = (PongWebSocketFrame) frame;
                // Process the received pong message
                handlePongFrame(ctx, pongFrame);
            } else if (frame instanceof CloseWebSocketFrame) {
                // Handle close frame
                CloseWebSocketFrame closeFrame = (CloseWebSocketFrame) frame;
                // Process the received close message
                // ...
            }
        }

        private void handlePingFrame(ChannelHandlerContext ctx, PingWebSocketFrame frame) {
            // 处理接收到的Ping消息
            ByteBuf data = frame.content();
            // 打印接收到的Ping消息
            System.out.println("Received ping: " + data.toString(CharsetUtil.UTF_8));
            // 构造Pong帧作为响应发送回客户端
            PongWebSocketFrame pongFrame = new PongWebSocketFrame(data);
            ctx.writeAndFlush(pongFrame);
        }

        private void handlePongFrame(ChannelHandlerContext ctx, PongWebSocketFrame frame) {
            // 处理接收到的Pong消息
            ByteBuf data = frame.content();
            // 打印接收到的Pong消息
            System.out.println("Received pong: " + data.toString(CharsetUtil.UTF_8));
            // ...
        }

        @Override
        public void channelActive(ChannelHandlerContext ctx) throws Exception {
            // WebSocket connection established
            //
            System.out.println("WebSocket connection established: " + ctx.channel());
            wbschannels.add(ctx.channel());
        }

        @Override
        public void channelInactive(ChannelHandlerContext ctx) throws Exception {
            // WebSocket connection closed
            System.out.println("WebSocket connection closed: " + ctx.channel());
            wbschannels.remove(ctx.channel());
        }

        @Override
        public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
            cause.printStackTrace();
            ctx.close();
        }

        private void handleWebSocketHandshake(ChannelHandlerContext ctx, FullHttpRequest request) {
            WebSocketServerHandshakerFactory wsFactory = new WebSocketServerHandshakerFactory(
                    getWebSocketURL(request), null, false);
            handshaker = wsFactory.newHandshaker(request);
            if (handshaker == null) {
                WebSocketServerHandshakerFactory.sendUnsupportedVersionResponse(ctx.channel());
            } else {
                handshaker.handshake(ctx.channel(), request);
            }
        }

        private String getWebSocketURL(FullHttpRequest request) {
            String url = "ws://" + request.headers().get(HttpHeaderNames.HOST) + request.uri();
            return url;
        }
    }

    private class HttpServerHandler extends SimpleChannelInboundHandler<FullHttpRequest> {

        private static final int DEFAULT_KEEP_ALIVE_TIMEOUT = 60;
        private Map<String, Integer> nsidsup = new HashMap<>();

        @Override
        protected void channelRead0(ChannelHandlerContext ctx, FullHttpRequest request) throws Exception {
            URI tturi = new URI(request.uri());
            String ttpath = tturi.getPath();

            boolean isKeepAlive = HttpUtil.isKeepAlive(request);
            if (request.uri().equals("/")) {
                //String htmlContent = "<html><body><h1>Hello, World!</h1></body></html>";
                FullHttpResponse response = new DefaultFullHttpResponse(
                        HttpVersion.HTTP_1_1,
                       // HttpResponseStatus.OK,
                        HttpResponseStatus.FOUND,
                        //Unpooled.copiedBuffer(htmlContent, java.nio.charset.StandardCharsets.UTF_8)
                        Unpooled.EMPTY_BUFFER
                );
                //response.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/html");
                //response.headers().set(HttpHeaderNames.CONTENT_LENGTH, response.content().readableBytes());
                response.headers().set(HttpHeaderNames.LOCATION, "/index.html");
                ctx.writeAndFlush(response).addListener(ChannelFutureListener.CLOSE);
            } else if (request.headers().contains(HttpHeaderNames.UPGRADE, HttpHeaderValues.WEBSOCKET, true)
                    && request.headers().contains(HttpHeaderNames.CONNECTION, HttpHeaderValues.UPGRADE, true)) {
                QueryStringDecoder decoder = new QueryStringDecoder(request.uri());
                Map<String, List<String>> params = decoder.parameters();
                String sid = "";
                if (params.containsKey("sid")) {
                    sid = params.get("sid").get(0);
                }
                // Handle WebSocket handshake
                handleWebSocketHandshake(ctx, request);
                if(!sid.isEmpty())nsidsup.put(sid, 1);
                //websktshak = 1;
            }else if (ttpath.equals("/socket.io/")){
                String eio = "";
                String transport = "";
                String getrestval = "";
                String sid = "";

                QueryStringDecoder decoder = new QueryStringDecoder(request.uri());
                Map<String, List<String>> params = decoder.parameters();
                if (params.containsKey("EIO")) {
                    eio = params.get("EIO").get(0);
                }
                if (params.containsKey("transport")) {
                    transport = params.get("transport").get(0);
                }
                if (params.containsKey("sid")) {
                    sid = params.get("sid").get(0);
                }
                if (isKeepAlive ){
                    if(ctx.channel().attr(KEEP_ALIVE_TIMEOUT).get() == null)
                        ctx.channel().attr(KEEP_ALIVE_TIMEOUT).set(DEFAULT_KEEP_ALIVE_TIMEOUT);

                }
                if(request.method().equals(HttpMethod.POST)){
                    System.out.println("post: " + request.uri());
                    String htmlContent = "ok";
                    FullHttpResponse response = new DefaultFullHttpResponse(
                            HttpVersion.HTTP_1_1,
                            HttpResponseStatus.OK,
                            Unpooled.copiedBuffer(htmlContent, java.nio.charset.StandardCharsets.UTF_8)
                    );
                    response.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/html");
                    response.headers().set(HttpHeaderNames.CONTENT_LENGTH, response.content().readableBytes());
                    if (isKeepAlive) {
                        response.headers().set(HttpHeaderNames.CONNECTION, HttpHeaderValues.KEEP_ALIVE);
                        response.headers().set(HttpHeaderNames.KEEP_ALIVE, "timeout=" + DEFAULT_KEEP_ALIVE_TIMEOUT);
                        // 设置响应的"Connection"头为"keep-alive"
                        ctx.writeAndFlush(response)
                                .addListener(ChannelFutureListener.CLOSE_ON_FAILURE);
                    } else {
                        // 不启用持久连接
                        ctx.writeAndFlush(response)
                                .addListener(ChannelFutureListener.CLOSE);
                    }
                    //ctx.writeAndFlush(response).addListener(ChannelFutureListener.CLOSE);
                    return;
                }
                String socketId = ctx.channel().id().asShortText();
                if(sid.isEmpty()){

                    getrestval = "0{\"sid\":\"" + socketId + "\",\"upgrades\":[\"websocket\"],\"pingInterval\":25000,\"pingTimeout\":60000}";

                }else{
                    if(!nsidsup.containsKey(socketId)) getrestval = "40{\"sid\":\"" + socketId + "\"}";
                        else
                            getrestval = "6";
                }

                FullHttpResponse response = new DefaultFullHttpResponse(
                        HttpVersion.HTTP_1_1,
                        HttpResponseStatus.OK,
                        Unpooled.copiedBuffer(getrestval, java.nio.charset.StandardCharsets.UTF_8)
                );
                response.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/plain");
                response.headers().set(HttpHeaderNames.CONTENT_LENGTH, response.content().readableBytes());
                if (isKeepAlive) {
                    response.headers().set(HttpHeaderNames.CONNECTION, HttpHeaderValues.KEEP_ALIVE);
                    response.headers().set(HttpHeaderNames.KEEP_ALIVE, "timeout=" + DEFAULT_KEEP_ALIVE_TIMEOUT);
                    // 设置响应的"Connection"头为"keep-alive"
                    ctx.writeAndFlush(response)
                            .addListener(ChannelFutureListener.CLOSE_ON_FAILURE);
                } else {
                    // 不启用持久连接
                    ctx.writeAndFlush(response)
                            .addListener(ChannelFutureListener.CLOSE);
                }
                //ctx.writeAndFlush(response).addListener(ChannelFutureListener.CLOSE);

            }
            else {
                System.out.println("url: " + request.uri());
                //serveStaticFile(ctx, request);
                serveresourceFile(ctx, request);
            }
        }

        @Override
        public void channelInactive(final ChannelHandlerContext ctx) throws Exception {

            Integer timeout = (Integer) ctx.channel().attr(KEEP_ALIVE_TIMEOUT).get();
            if (timeout != null && timeout > 0) {

                ctx.channel().eventLoop().schedule(new Runnable() {
                    @Override
                    public void run() {
                        if (ctx.channel().isActive()) {
                            ctx.channel().close();
                        }
                    }
                }, timeout, TimeUnit.SECONDS);
            }
        }

        private void handleWebSocketHandshake(ChannelHandlerContext ctx, FullHttpRequest request) {
            String websocketURL = "ws://" + request.headers().get(HttpHeaderNames.HOST) + request.uri();
            WebSocketServerHandshakerFactory handshakerFactory = new WebSocketServerHandshakerFactory(
                    websocketURL, null, false);
            WebSocketServerHandshaker handshaker = handshakerFactory.newHandshaker(request);
            if (handshaker == null) {
                WebSocketServerHandshakerFactory.sendUnsupportedVersionResponse(ctx.channel());
            } else {
                handshaker.handshake(ctx.channel(), request);
            }
        }

        private void serveStaticFile(ChannelHandlerContext ctx, FullHttpRequest request) throws Exception {
            String uri = request.uri();
            String filePath = staticFilesPath + uri;
            File file = new File(filePath);
            if (file.exists() && file.isFile()) {
                RandomAccessFile raf = new RandomAccessFile(file, "r");
                long fileLength = raf.length();

                HttpResponse response = new DefaultHttpResponse(HttpVersion.HTTP_1_1, HttpResponseStatus.OK);
                HttpUtil.setContentLength(response, fileLength);
                setContentTypeHeader(response, file);

                if (HttpUtil.isKeepAlive(request)) {
                    response.headers().set(HttpHeaderNames.CONNECTION, HttpHeaderValues.KEEP_ALIVE);
                }

                ctx.write(response);
                ctx.write(new DefaultFileRegion(raf.getChannel(), 0, fileLength), ctx.newProgressivePromise());
                ChannelFuture future = ctx.writeAndFlush(LastHttpContent.EMPTY_LAST_CONTENT);
                if (!HttpUtil.isKeepAlive(request)) {
                    future.addListener(ChannelFutureListener.CLOSE);
                }
            } else {
                sendNotFoundResponse(ctx);
            }
        }

        private void serveresourceFile(ChannelHandlerContext ctx, FullHttpRequest request) throws Exception {

            String uri = request.uri().substring(1);

            //System.out.println("res: " + uri);
            ClassLoader classLoader = getClass().getClassLoader();
            InputStream inputStream = classLoader.getResourceAsStream(uri);

            if(inputStream == null) {
                System.out.println("res nofound: " + uri);
                sendNotFoundResponse(ctx);
                return;
            }

            long fileLength = inputStream.available();

            HttpResponse response = new DefaultHttpResponse(HttpVersion.HTTP_1_1,HttpResponseStatus.OK);

            HttpUtil.setContentLength(response, fileLength);

            //setContentTypeHeader(response, determineContentType(uri));
            response.headers().set(HttpHeaderNames.CONTENT_TYPE, determineContentType(uri));

            if(HttpUtil.isKeepAlive(request)) {
                response.headers().set(HttpHeaderNames.CONNECTION, HttpHeaderValues.KEEP_ALIVE);
            }

            ctx.write(response);

            ReadableByteChannel channel = Channels.newChannel(inputStream);
            ChannelFuture future = ctx.writeAndFlush(new HttpChunkedInput(new ChunkedNioStream(channel)), ctx.newProgressivePromise());
            if (!HttpUtil.isKeepAlive(request)) {
                future.addListener(ChannelFutureListener.CLOSE);
            }

        }



        private String determineContentType(String resource) {

            if(resource.endsWith(".html")) {
                return "text/html";
            } else if(resource.endsWith(".js")) {
                return "text/javascript";
            } else if(resource.endsWith(".css")) {
                return "text/css";
            }

            // 其他类型判断

            return "application/octet-stream";
        }

        private void setContentTypeHeader(HttpResponse response, File file) {
            String mimeType = "application/octet-stream";
            String fileName = file.getName();
            int dotIndex = fileName.lastIndexOf('.');
            if (dotIndex != -1) {
                String fileExtension = fileName.substring(dotIndex + 1).toLowerCase();
                // Additional MIME type mappings
                if (fileExtension.equals("txt")) {
                    mimeType ="text/plain";
                } else if (fileExtension.equals("html") || fileExtension.equals("htm")) {
                    mimeType = "text/html";
                } else if (fileExtension.equals("css")) {
                    mimeType = "text/css";
                } else if (fileExtension.equals("js")) {
                    mimeType = "application/javascript";
                } else if (fileExtension.equals("json")) {
                    mimeType = "application/json";
                } else if (fileExtension.equals("png")) {
                    mimeType = "image/png";
                } else if (fileExtension.equals("jpg") || fileExtension.equals("jpeg")) {
                    mimeType = "image/jpeg";
                } else if (fileExtension.equals("gif")) {
                    mimeType = "image/gif";
                }
            }
            response.headers().set(HttpHeaderNames.CONTENT_TYPE, mimeType);
        }

        private void sendNotFoundResponse(ChannelHandlerContext ctx) {
            FullHttpResponse response = new DefaultFullHttpResponse(
                    HttpVersion.HTTP_1_1,
                    HttpResponseStatus.NOT_FOUND
            );
            ctx.writeAndFlush(response).addListener(ChannelFutureListener.CLOSE);
        }

        @Override
        public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
            cause.printStackTrace();
            ctx.close();
        }
    }

    public static void main(String[] args) throws Exception {
        int port = 8087;
        String staticFilesPath = "D:\\temp\\public";

        HttpAndWebSocketServer server = new HttpAndWebSocketServer(port, staticFilesPath);
        server.start();
    }
}

