package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.echo.LineMessageEncoderDecoder;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        // TODO: implement this

        /*
         * MUST CHANGE "encode and decode" in LIneMessageEncoderDecoder
         */
        if (args[1].equals("tpc")) {
            Server.threadPerClient(
                    Integer.parseInt(args[0]), // port
                    StompMesasgingProtocolx::new, // protocol factory
                    MessageEncoderDecoderx::new // message encoder decoder factory
            ).serve();
        }

        if (args[1].equals("reactor")) {
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    Integer.parseInt(args[0]), // port
                    StompMesasgingProtocolx::new, // protocol factory
                    MessageEncoderDecoderx::new // message encoder decoder factory
            ).serve();
        }
    }
}
