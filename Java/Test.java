import static org.junit.Assert.*;

import java.util.HashMap;


public class Test {

	@org.junit.Test
	public void testUserClass() {
		User testUser = new User("UserName", "FS", 100);
		assertTrue(testUser.getCredit() == 100);
		assertTrue(testUser.getName().equals("UserName"));
		assertTrue(testUser.getType().equals("FS"));
		testUser.setCredit(99);
		assertTrue(testUser.getCredit() == 99);
	}
	
	@org.junit.Test
	public void testEventClass() {
		Event testEvent = new Event("Event", "UserName", 300, 3.50f);
		assertTrue(testEvent.getTitle().equals("Event"));
		assertTrue(testEvent.getPrice() == 3.50f);
		assertTrue(testEvent.getSeller().equals("UserName"));
		assertTrue(testEvent.getAmount() == 300);
		testEvent.setAmount(295);
		testEvent.setPrice(3.55f);
	}
	
	@org.junit.Test
	public void testBackEnd() {
		BackEnd test = new BackEnd();
		
		String[] args = {""};
		test.main(args);
		
	}
	
	//Test first for loop in writeToFile
	@org.junit.Test 
	public void loopCoverage() {
		BackEnd test = new BackEnd();
		User userOne = new User("nameOne", "FS", 100);
		User userTwo = new User("nameTwo", "FS", 100);
		User userThree = new User("nameThree", "FS", 100);
		Event event = new Event("Event", "nameOne", 300, 3.50f);
		
		HashMap<String, User> users = new HashMap<String, User>();
		HashMap<String, Event> events = new HashMap<String, Event>();
		events.put("Event", event);
		
		//Loop iterates 0 times, empty hash map
		test.writeToFile(users, events);
		
		//Loop iterates 1 time, one user in hash map
		users.put(userOne.getName(), userOne);
		test.writeToFile(users, events);
		
		//Loop iterates 2 times, two users in hash map
		users.put(userTwo.getName(), userTwo);
		
		//Loop iterates 3 times, three users in hash map
		users.put(userThree.getName(), userThree);
	}
	
}
