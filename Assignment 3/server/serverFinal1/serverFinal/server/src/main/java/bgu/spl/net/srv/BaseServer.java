package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;
import bgu.spl.net.impl.stomp.MessageEncoderDecoderx;
import bgu.spl.net.impl.stomp.StompMesasgingProtocolx;

public abstract class BaseServer<T> implements Server<T> {

    private final int port;
    private final Supplier<StompMesasgingProtocolx<T>> protocolFactory;
    private final Supplier<MessageEncoderDecoderx<T>> encdecFactory;
    private ServerSocket sock;
    //Rahaf
    //private Connectionsx con;



    public BaseServer(
            int port,
            Supplier<StompMesasgingProtocolx<T>> protocolFactory,
            Supplier<MessageEncoderDecoderx<T>> encdecFactory) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
        this.sock = null;
        //this.con= new Connectionsx<>();
    }

    @Override
    public void serve() {

        try (ServerSocket serverSock = new ServerSocket(port)) {
            System.out.println("Server started");

            this.sock = serverSock; // just to be able to close

            while (!Thread.currentThread().isInterrupted()) {

                Socket clientSock = serverSock.accept();

                BlockingConnectionHandler<T> handler = new BlockingConnectionHandler<>(
                        clientSock,
                        encdecFactory.get(),
                        protocolFactory.get()
                        );


                execute(handler);
            }
        } catch (IOException ex) {
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
        if (sock != null)
            sock.close();
    }

    protected abstract void execute(BlockingConnectionHandler<T> handler);

}
