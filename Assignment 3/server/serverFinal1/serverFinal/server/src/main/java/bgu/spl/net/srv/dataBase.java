package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;

public class dataBase {
    public Connectionsx connections;
    private static dataBase instance = null;
    // <username,password>
    private ConcurrentHashMap<String, String> usernames;
    // <username, online/offline>
    private ConcurrentHashMap<String, Boolean> isOnline;
    // <ChannelName, List<usernames>>
    private ConcurrentHashMap<String, LinkedBlockingQueue<String>> channels;
    // <username, <Subscription_id, ChannelName>>
    private ConcurrentHashMap<String, ConcurrentHashMap<Integer, String>> channelsIds;

    private dataBase() {
        usernames = new ConcurrentHashMap<>();
        isOnline = new ConcurrentHashMap<>();
        channels = new ConcurrentHashMap<>();
        channelsIds = new ConcurrentHashMap<>();
    }

    public static synchronized dataBase getInstance() {
        if (instance == null) {
            instance = new dataBase();
        }
        return instance;
    }

    public LinkedBlockingQueue<String> getSubscribed(String channel) {
        return channels.get(channel);
    }

    // messga frame
    private String messageFrame(int subscription, int messageId, String channel, String message) {
        return "MESSAGE\nsubscription:" + subscription + "\nmessage-id:" + messageId + "\ndestination:" + channel + "\n"
                + message + "\n";
    }

    // error frame
    private String errorFrame(int receicptId, String message, String frame, String errorMessage) {
        if (receicptId == -1)
            return "ERROR\nmessage: " + message + "\nThe message:\n-----\n" + frame + "\n-----\n" + errorMessage;
        return "ERROR\nreceipt-id:" + receicptId + "\nmessage: " + message + "\nThe message:\n-----\n" + frame
                + "\n-----\n" + errorMessage;
    }

    // connected frame
    private String connectedFrame() {
        return "CONNECTED\nversion :1.2\n\n";
    }

    // recipte Frame
    private String receiptFrame(int receicptId) {
        return "RECEIPT\nreceipt-id:" + receicptId + "\n\n";
    }

    // login
    public String login(String frame, String username, String password) {
        // if the username is new then add it to the usernames
        if (!usernames.containsKey(username)) {
            // add the username to the usernames hashmap
            usernames.put(username, password);
            // make username online(isOnline = true)
            isOnline.put(username, true);
            // return connected frame

            return connectedFrame();
        }
        // if the password is wrong reutrn an error frame
        if (!usernames.get(username).equals(password))
            return errorFrame(-1, "Wrong password", frame, "you've enterd a wrong password for" + username);
        // if the username isnt new and the password is corrent then make the username
        // online
        isOnline.put(username, true);
        // return a connected frame
        return connectedFrame();
    }

    // logout
    public String logout(String username, int receiptId) {
        // make the username offline (isOnline = false)
        isOnline.put(username, false);
        // return the recipt frame
        return receiptFrame(receiptId);
    }

    // join
    public String subscribe(String username, String channel, int subscription_id, int receiptId){
        // check if theres an already existed channel if add it to channels
        if (!channels.containsKey(channel)) {
            channels.put(channel, new LinkedBlockingQueue<>());
        }
        // if the username isnt subscribed to the channel already add it to the channel
        if (!channels.get(channel).contains(username)) {
            try {
                channels.get(channel).put(username);
            } catch (InterruptedException e) {
            }
        }
        // return recipt frame to the client
        return receiptFrame(receiptId);
    }

    // exit
    public String unSubscribe(String username, int subscription_id, String frame, int receiptId) {

        // if username didnt subscribe to this channel he cant unsubscribe from it
        if (!channelsIds.get(username).containsKey(subscription_id)) {
            return errorFrame(receiptId, username + "isnt subscribed to this channel", frame,
                    "You tried to unsubscribe from a channel you arent subscribed to");
        }
        String channel = channelsIds.get(username).get(subscription_id);
        // if the channel doesnt exist then you cant unsubscribe to that channel
        if (!channels.containsKey(channel)) {
            return errorFrame(receiptId, "Channel doesn't exists!", frame,
                    "You have attempted to unsubscribe from a non existing channel");
        }
        // if the user name isnt subscribred to this channel return an error frame
        if (!channels.get(channel).contains(username)) {
            return errorFrame(receiptId, "Trying to unsubscribe from channel that isnt subscribed to", frame,
                    "You have attempted to unsubscribe of the channel " + channel
                            + "\nBut you never subscribed tp it!");
        }
        // remove the username from the channel
        channels.get(channel).remove(username);
        // return a recipt frame
        return receiptFrame(receiptId);
    }
    // send message

    public String sendMessage(String username, int subscription_id, String message, String frame) {
        // String that ahs has the channel
        String channel = channelsIds.get(username).get(subscription_id);
        // if you are not subscribed to this channel you cant send a messgae to it
        if (!channels.get(channel).contains(username))
            return errorFrame(-1, "User not subscribed to channel!", frame,
                    "You have attempted to send a message to\nchannel , but you aren't subscribed to it yet!");

        //you did not implement it in Connectionsx
        connections.send(channel, messageFrame(subscription_id, 0, channel, message));
      //connections.send(channel, messageFrame(subscription_id, 0, channel, message));

        return "";
    }
}
