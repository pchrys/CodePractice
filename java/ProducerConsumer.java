import java.util.LinkedList;
import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.locks.*;
import java.util.concurrent.*;

public class ProducerConsumer {

  private LinkedList<Long> buffer = new LinkedList<>();
  private Lock lock = new ReentrantLock();
  private Condition full = lock.newCondition();
  private Condition empty = lock.newCondition();

  private final int THREAD_NUM = 1024;
  ExecutorService es1 = Executors.newFixedThreadPool(THREAD_NUM);
  ExecutorService es2 = Executors.newFixedThreadPool(THREAD_NUM);

  private final int CAP = THREAD_NUM * 10;
  ProducerConsumer() {}

  Runnable r1 = () -> {
    while (true) {
      try {
        lock.lock();
        long id = Thread.currentThread().getId();
        while (buffer.size() >= CAP) {
          System.out.printf("producer %d is put on wait \n", id);
          full.await();
        }

        buffer.add(id);
        empty.signalAll();
      } catch (Exception e) {
        e.printStackTrace();
      } finally {
        lock.unlock();
      }
    }
  };

  Runnable r2 = () -> {
    while (true) {
      try {
        lock.lock();
        long id = Thread.currentThread().getId();

        while (buffer.isEmpty()) {
          System.out.printf("consumer %d is put on wait \n", id);
          empty.await();
        }

        System.out.printf("%d is consumed by thread %d \n", buffer.pollFirst(),
                          id);

        full.signalAll();
	//Thread.sleep(100);
      } catch (Exception e) {
        e.printStackTrace();
      } finally {
        lock.unlock();
      }
    }
  };

  public void startProducerAndConsumer() {
    for (int i = 0; i < THREAD_NUM; i++) {
      es1.submit(r1);
      es2.submit(r2);
    }
  }

  public static void main(String[] args) {

    ProducerConsumer pc = new ProducerConsumer();

    pc.startProducerAndConsumer();
  }
}
