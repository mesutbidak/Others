package sync;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Semaphore;

public class Test {
	public static void main(String [] args) {
		ExecutorService executorService = Executors.newCachedThreadPool();
		ReadWriteLock RW = new ReadWriteLock();
		
		executorService.execute(new Writer(RW));
		executorService.execute(new Writer(RW));
		executorService.execute(new Writer(RW));
		executorService.execute(new Writer(RW));
		
		
		executorService.execute(new Reader(RW));
		executorService.execute(new Reader(RW));
		executorService.execute(new Reader(RW));
		executorService.execute(new Reader(RW));
		
	}
}


class ReadWriteLock{
	private Semaphore S=new Semaphore(1);
	public int reader_counter=0;
	
	public void readLock() throws InterruptedException {
		S.acquire();
		reader_counter++;
		S.release();
	}
	public void writeLock() throws InterruptedException {
		while(true) {
			S.acquire();
			if(reader_counter==0) {
				break;
			}
			else {
				S.release();
			}
		}
	}
	public void readUnLock() throws InterruptedException {
		S.acquire();
		reader_counter--;
		S.release();
	}
	public void writeUnLock() {
		S.release();
	}

}




class Writer implements Runnable
{
   private ReadWriteLock RW_lock;
  
   public Writer(ReadWriteLock rw) {
    	RW_lock = rw;
   }

    public void run() {
      while (true){
    	try {
			RW_lock.writeLock();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
    	

    	RW_lock.writeUnLock();
       
      }
   }


}



class Reader implements Runnable
{
   private ReadWriteLock RW_lock;
   
   public Reader(ReadWriteLock rw) {
    	RW_lock = rw;
    	
   }
   public void run() {
      while (true){
    	try {
			RW_lock.readLock();
		}catch (InterruptedException e) {
			e.printStackTrace();
		}                                                
    	

    	try {
			RW_lock.readUnLock();
		}catch (InterruptedException e) {
			e.printStackTrace();
		}
      }
   }


}