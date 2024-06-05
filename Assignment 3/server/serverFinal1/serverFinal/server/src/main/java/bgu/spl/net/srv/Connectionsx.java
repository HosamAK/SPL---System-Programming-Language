package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;

public class Connectionsx<T> implements Connections<T>{
    private ConcurrentHashMap<Integer,ConnectionHandler<T>> chm;//clients 
    private ConcurrentHashMap<String,ConnectionHandler<T>> chmu;//usres
    
    AtomicInteger id;

    public Connectionsx(){
        chm = new ConcurrentHashMap<>();
        //chmu = new ConcurrentHashMap<>();
        id = new AtomicInteger(0);

    }

    @Override
    public boolean send(int connectionId, T msg) {

        if(!chm.containsKey(connectionId) || msg == null){
            return false;
        }
        chm.get(connectionId).send(msg);
        return true;
    }

    @Override
    public void send(String channel, T msg) {
        LinkedBlockingQueue<String> subs = dataBase.getInstance().getSubscribed(channel);
        for(String user: subs){
            if(chmu.contains(user)){
                chmu.get(user).send(msg);
            }
        }
    }


    @Override
    public void disconnect(int connectionId) {
        // if(chmu.contains(connectionId))
        // //remove the id from userss hash map 
        //     chmu.remove(connectionId);
            
        if(chm.contains(connectionId))
            //remove the id from clients hash maps 
            chm.remove(connectionId);
    }

    //add the client to the hash map 
    public int add( ConnectionHandler<T> cHandler){
        int tepmId = id.getAndIncrement();
        chm.put(tepmId, cHandler);
        //System.out.println("Added " + tepmId);
        //System.out.println(this);
        return tepmId;
    }
    private static class ClassicSingleton {
        public static Connections SingletonHolder = new Connectionsx<>();// must check the string
    }
    public static Connections getInstance() {
        return ClassicSingleton.SingletonHolder;
    }

}