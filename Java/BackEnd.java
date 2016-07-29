//Input: TransactionFile.txt, UsersFile.txt, EventsFile.txt
//Output: TransactionFile.txt(Empty), UsersFile.txt(Updated), EventsFile.txt(Updated)
//Purpose: This program will take the log from the transaction file and then update
//		   the Users and Events file with any change from the transaction. The 
//		   transaction file will be blanked afterward for next day.

import java.lang.NullPointerException;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.HashMap;

public class BackEnd{
	static final String UsersFile = "UsersFile.txt";
	static final String EventsFile = "EventsFile.txt";
	static final String TransactionFile = "TransactionFile.txt";
	static BufferedReader Users;
	static BufferedReader Events;
	static BufferedReader Transaction;

	public static void main(String[] Argv){
		HashMap<String, User> userData = new HashMap<String, User>();
		HashMap<String, Event> eventData = new HashMap<String, Event>();
		
		readFromFile();
		readData(userData, eventData);
		applyTransaction(userData, eventData);
		writeToFile(userData, eventData);
	}
	
	//Responsible for reading each transaction and applying it to the user and event data
	//Require reference to both user and event hashmap as argument to modify
	//Returns 0 if successful, -1 if failed.
	public static int applyTransaction(HashMap<String, User> uData, HashMap<String, Event> eData){
		String line;
		String name;
		float value;
		int transType;
		int amount;
		User u;
		Event e;
		
		try{
			//Gets each transaction one at a time and apply the change to the user or event database
			line = Transaction.readLine();
			while(line != null){
				transType = Integer.parseInt(line.substring(0, 3));
				
				if(transType == 1){ //Creating an account
					name = line.substring(3, 18).trim();
					
					if(uData.get(name) != null){
						System.out.println("ERROR: The user " + name + " already exist in the database.");
					} else {
						value = Float.parseFloat(line.substring(22, 31));
						
						if(value > 999999){
							System.out.println("ERROR: User " + name + " maximum credit cannot be greater than $999,999.");
							continue;
						}
						
						u = new User(name, line.substring(19,21), value);						
						uData.put(name, u);
					}
				} else if(transType == 2){	//Deleting an account
					name = line.substring(3, 18).trim();
					u = uData.remove(name);
					
					if(u == null){
						System.out.println("ERROR: The user " + name + " cannot be removed because it no longer exist.");
					}
				} else if(transType == 3){	//Selling tickets for new event
					name = line.substring(3, 22).trim();
					e = eData.get(name);
					if(e != null){
						System.out.println("ERROR: Event " + name + " already existed.");
						continue;
					}
					
					String seller = line.substring(23, 36).trim();
					if(uData.get(seller) == null){
						System.out.	println("ERROR: Seller " + seller + " does not exist, event not registered.");
						continue;
					}
					
					amount = Integer.parseInt(line.substring(37, 40));
					if(amount > 100){
						System.out.println("ERROR: Event " + name + " cannot have more than 100 tickets for sale.");
						continue;
					}
					
					value = Float.parseFloat(line.substring(41, 47));
					if(value > 999.99){
						System.out.println("ERROR: Event " + name + " cannot have price greater than $999.99.");
						continue;
					}
					
					e = new Event(name, seller, amount, value);
					
					eData.put(name, e);
				} else if(transType == 4){	//Buying from an event
					String buyer = line.substring(23, 36).trim();
					u = uData.get(buyer);
					if(u == null){
						System.out.println("ERROR: Buyer " + buyer + " does not exist, sale cancelled.");
						continue;
					}
					
					name = line.substring(3, 22).trim();
					e = eData.get(name);
					if(e == null){
						System.out.println("ERROR: Event " + name + "does not exist, sale cancelled.");
						continue;
					}
					
					amount = Integer.parseInt(line.substring(37, 40));
					if(amount > e.getAmount()){
						System.out.println("ERROR: Event " + name + " do not have " + amount + " of tickets for sale.");
						continue;
					}
					
					if(amount > 4){
						System.out.println("ERROR: User " + buyer + " cannot buy more then 4 tickets at once.");
						continue;
					}
					
					if(amount * e.getPrice() > u.getCredit()){
						System.out.println("ERROR: User " + buyer + " lack the fund to buy tickets from " + name + ".");
						continue;
					}
					
					u = uData.remove(buyer);
					e = eData.remove(name);
					
					e.setAmount(e.getAmount() - amount);
					u.setCredit(u.getCredit() - (e.getPrice() * amount));
					
					uData.put(buyer, u);
					eData.put(name, e);
				} else if(transType == 5){	//Refund from seller to buyer
					value = Float.parseFloat(line.substring(35, 44));
					
					name = line.substring(3, 18).trim();
					u = uData.remove(name);
					
					if(u == null){
						System.out.println("ERROR: The user " + name + " cannot be refunded because it no longer exist.");
						//No point in subtracting the fund from seller if the buyer no longer exist
					} else {
						u.setCredit(u.getCredit() + value);
						uData.put(name, u);
						
						name = line.substring(19, 34).trim();
						u = uData.remove(name);
						
						if(u == null){
							System.out.println("ERROR: The user " + name + " cannot be initiate the refund because it no longer exist.");
							
							//If the seller is gone, we can't just give buyer free money
							name = line.substring(3, 18).trim();
							u = uData.remove(name);
							u.setCredit(u.getCredit() - value);
							uData.put(name, u);
						} else {
							u.setCredit(u.getCredit() - value);
							uData.put(name, u);
						}
					}
				} else if(transType == 6){	//Add credit to an account
					name = line.substring(3, 18).trim();
					u = uData.get(name);
					
					if(u == null){
						System.out.println("ERROR: Credit cannot be added to the user " + name + " because it no longer exist.");
					} else {
						value = Float.parseFloat(line.substring(22, 31));
						
						if(value > 1000){
							System.out.println("ERROR: User " + name + " cannot add more than $1000 in a single transaction.");
							continue;
						}
						
						if(value + u.getCredit() > 999999){
							System.out.println("ERROR: User " + name + " cannot add anymore credit due to reaching account limit.");
							continue;
						}
						
						u = uData.remove(name);
						u.setCredit(u.getCredit() + value);
						uData.put(name, u);
					}
				}
				
				line = Transaction.readLine();
			}
			
			Transaction.close();

			return 0;
		} catch (IOException ex){
			System.out.println("ERROR: IO Exception???? Should never occur!");
			return -1;
		} catch (NullPointerException exp){
			System.out.println("ERROR: Null pointer? Looks like you forgot to read from files.");
			return -1;
		}
	}
	
	//Responsible for reading in the data from the readers and putting it on the hashmap
	//Require reference to both user and event hashmap as argument to write into
	//Returns 0 if successful, -1 if failed.
	public static int readData(HashMap<String, User> uData, HashMap<String, Event> eData){
		String name;
		String line;
		float value;
		int amount;
		User u;
		Event e;
		
		try{
			//Gets all the user data from the file and put it in the hash map one line at a time
			line = Users.readLine();
			while(line != null){
				name = line.substring(0, 15).trim();
				value = Float.parseFloat(line.substring(19,28));
				u = new User(name, line.substring(16, 18), value);
				
				uData.put(name, u);
				line = Users.readLine();
			}
			
			//Gets all the event data from the file and put it in a hash map
			line = Events.readLine();
			while(line != null){
				name = line.substring(0, 19).trim();
				amount = Integer.parseInt(line.substring(34, 37));
				value = Float.parseFloat(line.substring(38,44));
				e = new Event(name, line.substring(20, 33).trim(), amount, value);
				
				eData.put(name, e);
				line = Events.readLine();
			}
			
			Users.close();
			Events.close();
			
			return 0;
		} catch (IOException ex){
			System.out.println("ERROR: IO Exception???? Should never occur!");
			return -1;
		} catch (NullPointerException exp){
			System.out.println("ERROR: Null pointer? Looks like you forgot to read from files.");
			return -1;
		}
	}
	
	//Read the filename as global variables and create a new reader for it
	//Readers are global, so there is no need to return them
	//Returns 0 if successful, -1 if failed.
	public static int readFromFile(){
		try{
			Users = new BufferedReader(new FileReader(UsersFile));
			Events = new BufferedReader(new FileReader(EventsFile));
			Transaction = new BufferedReader(new FileReader(TransactionFile));
			
			return 0;
		} catch (FileNotFoundException e){
			System.out.println("ERROR: Files are not found!");
			return -1;
		}
	}
	
	//Write all the data inside the given hashmap into the files
	//Files are provided as global variables
	//Hashmap for user and event data are require as argument for reading
	//Returns 0 is successful, -1 if failed.
	public static int writeToFile(HashMap<String, User> u, HashMap<String, Event> e){
		User[] userList = u.values().toArray(new User[0]);
		Event[] eventList = e.values().toArray(new Event[0]);
		int userSize = userList.length;
		int eventSize = eventList.length;
		int tempSize;
		String tempStr;
		char[] strBuff;
		BufferedWriter Users;
		BufferedWriter Events;
		
		try{
			Users = new BufferedWriter(new FileWriter(UsersFile));
			Events = new BufferedWriter(new FileWriter(EventsFile));
		
			//Handling outputting to the user data file
			for(int i = 0; i < userSize; i++){
				strBuff = new char[29];
				tempStr = userList[i].getName();
				tempSize = tempStr.length();
				
				//Starting with the username
				for(int j = 0; j < tempSize; j++){
					strBuff[j] = tempStr.charAt(j);
				}
				
				//Then empty space for padding
				for(int j = tempSize; j < 15; j++){
					strBuff[j] = ' ';
				}
				
				//Then spaces and the type follow by the space
				strBuff[15] = ' ';
				tempStr = userList[i].getType();
				strBuff[16] = tempStr.charAt(0);
				strBuff[17] = tempStr.charAt(1);
				strBuff[18] = ' ';
				
				tempStr = userList[i].getCredit() + "";

				//Then the 0 padding for the numerals
				for(int j = 27 - tempStr.length(); j > 18; j--){
					strBuff[j] = '0';
				}
				
				//Finally the actual numbers itself
				for(int j = 28 - tempStr.length(); j < 28; j++){
					strBuff[j] = tempStr.charAt(j);
				}
				
				strBuff[28] = '\n';
				
				Users.write(strBuff, 0, 29);
			}
			Users.close();
			
			//Handling outputting to the event data file
			for(int i = 0; i < eventSize; i++){
				strBuff = new char[46];
				tempStr = eventList[i].getTitle();
				tempSize = tempStr.length();
				
				//Starting with the event title
				for(int j = 0; j < tempSize; j++){
					strBuff[j] = tempStr.charAt(j);
				}
				
				//Then empty space for padding
				for(int j = tempSize; j < 19; j++){
					strBuff[j] = ' ';
				}
				
				//Then another space for separation
				strBuff[19] = ' ';
				tempStr = eventList[i].getSeller();
				tempSize = tempStr.length();
				
				//Then the username of the seller
				for(int j = 0; j < tempSize; j++){
					strBuff[j+20] = tempStr.charAt(j);
				}
				
				//Then more space for padding
				for(int j = tempSize; j < 13; j++){
					strBuff[j+20] = ' ';
				}
				
				//Then another space for separation
				strBuff[33] = ' ';
				tempStr = eventList[i].getAmount() + "";
				tempSize = tempStr.length();
				
				//Then the amount of the tickets available
				if(tempSize > 2){
					strBuff[34] = tempStr.charAt(0);
					strBuff[35] = tempStr.charAt(1);
					strBuff[36] = tempStr.charAt(2);
				} else if(tempSize > 1) {
					strBuff[34] = ' ';
					strBuff[35] = tempStr.charAt(0);
					strBuff[36] = tempStr.charAt(1);
				} else {
					strBuff[34] = ' ';
					strBuff[35] = ' ';
					strBuff[36] = tempStr.charAt(0);
				}
				
				tempStr = eventList[i].getPrice() + "";

				//Then the 0 padding for the price
				for(int j = 43 - tempStr.length(); j > 36; j--){
					strBuff[j] = '0';
				}
				
				//Finally the actual price itself
				for(int j = 44 - tempStr.length(); j < 44; j++){
					strBuff[j] = tempStr.charAt(j);
				}
				
				strBuff[44] = '\n';
				
				Events.write(strBuff, 0, 45);
			}
			Events.close();
			
			return 0;
		} catch (IOException ee) {
			System.out.println("ERROR: Permission error probably, idk.");
			return -1;
		}
	}
}

//This class is responsible for holding the data for each user
//The data it contains are:
//User's name, User's type and User's Credit
class User{
	String name;
	String type;
	float credit;
	
	public User(String n, String t, float c){
		name = n;
		type = t;
		credit = c;
	}
	
	//Various access function for the class
	public String getName(){
		return name;
	}
	
	public String getType(){
		return type;
	}
	
	public float getCredit(){
		return credit;
	}
	
	//Mutation method for credit only, other data do not ever needs to be changed
	public void setCredit(float c){
		credit = c;
	}
}

//This class is responsible for holding the data for each event
//The data it contains are:
//Event Name, Seller's Name, Available Tickets and Ticket Price
class Event{
	String title;
	String seller;
	float price;
	int amount;
	
	public Event(String t, String s, int a, float p){
		title = t;
		seller = s;
		amount = a;
		price = p;
	}
	
	//Various access function for the class
	public String getTitle(){
		return title;
	}
	
	public String getSeller(){
		return seller;
	}
	
	public int getAmount(){
		return amount;
	}
	
	public float getPrice(){
		return price;
	}
	
	//Mutation method for price and tickets amount only
	//The other data do not ever needs to be changed
	void setAmount(int a){
		amount = a;
	}
	
	void setPrice(float p){
		price = p;
	}
}