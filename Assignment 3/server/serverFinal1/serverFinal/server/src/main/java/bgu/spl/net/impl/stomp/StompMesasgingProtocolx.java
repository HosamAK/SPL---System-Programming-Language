package bgu.spl.net.impl.stomp;


import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.dataBase;

public class StompMesasgingProtocolx<T> implements StompMessagingProtocol<T> {

    private int connectionId;
    private String username = "";
    private Connections<T> connections;
    private boolean shouldTerminate = false;


    @Override
    public void start(int connectionId, Connections<T> connections) {
        // TODO Auto-generated method stub
        this.connectionId = connectionId;
        this.connections = connections;
    }

    @Override
    public void process(T message) {
        // TODO Auto-generated method stub
        // COMMAND\nargument:val\nargument:val\nargument:val\nargument:val\nframeBody:val\0
        String[] arguments = message.toString().split("\n");
        String out = "";
        int subscription_id;
        int receipt_id;

        switch (arguments[0]) {
            case "CONNECT":
            out = dataBase.getInstance().login(message.toString(), arguments[3].substring(arguments[3].indexOf(":")+1), arguments[4].substring(arguments[4].indexOf(":")+1));
            username = arguments[3];
            break;

            case "DISCONNECT":
            receipt_id = Integer.valueOf(arguments[1].substring(arguments[1].indexOf(":")+1));
            out = dataBase.getInstance().logout(username, receipt_id);
            shouldTerminate = true;
            break;

            case "SEND":
            subscription_id = Integer.valueOf(arguments[1].substring(arguments[1].indexOf(":")+1));
            out = dataBase.getInstance().sendMessage(username, subscription_id, arguments[2].substring(arguments[2].indexOf(":")+1), message.toString());
            break;
            case "SUBSCRIBE":
            subscription_id = Integer.valueOf(arguments[2].substring(arguments[2].indexOf(":")+1));
            receipt_id = Integer.valueOf(arguments[3].substring(arguments[3].indexOf(":")+1));
            out = dataBase.getInstance().subscribe(username, arguments[2].substring(arguments[2].indexOf(":")+1),subscription_id, receipt_id);
            break;
            case "UNSUBSCRIBE":
            subscription_id = Integer.valueOf(arguments[1].substring(arguments[1].indexOf(":")+1));
            receipt_id = Integer.valueOf(arguments[2].substring(arguments[2].indexOf(":")+1));
            out = dataBase.getInstance().unSubscribe(username, subscription_id, message.toString(), receipt_id);
            break;
            
        }
        if(!out.equals("")){
            System.out.println("response = " + (T)out);
            System.out.println("Sending to id " + connectionId + ", sent? " + connections.send(connectionId, (T)out));
        }

    }

    @Override
    public boolean shouldTerminate() {
        // TODO Auto-generated method stub
        return shouldTerminate;
    }


}
