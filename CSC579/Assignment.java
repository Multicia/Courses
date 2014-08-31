<<<<<<< HEAD
import java.util.Random;
import java.util.Arrays;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
public class Assignment
{
	private static final boolean enableDebug = false;
	private static final boolean slowDebug = false;
	
	//Queues
	static LinkedList Infinite_server_queue;
	static LinkedList Client_queue;
	//Assignment 3:
	static float[] end_to_end_delay;
	static float[] batch_95 = new float[30];
	static float[] mean = new float[30];
	static int received_packet=0;
	static int N_Deps=0;
	static int batch_size=0;
	static int N_total=0;
	static float T_mean=0;
	static float T_sd=0;
	static int num_of_batches=0;
	static float e2e_mean=0;
	static float delay_server_queue=0;
	
	//debug
	static int temp_count_client_queue=0;
	static float mean_client_queue=0;
	static int temp_count_isq=0;
	static float mean_isq=0;
	static int temp_count_sq=0;
	static float mean_sq=0;
	static int temp_count_no_cust=0;
	static int count_no_cust=0;
	
	
	
	static Random RandomGenerator;
	int Clock=0;
	//
	static boolean flag=true;
	static float nextDeparture=0;
	static float CLK=0;
	
	//Basic User Inputs
	static float N,D_H,D_L;
	static int T_L,T_H;
	static float InfiniteServerRate,ClientQueueRate;
	
	static float randomgenerator(float mean)
	{
		float number;
		//Exponential Distribution: http://stackoverflow.com/questions/2106503/pseudorandom-number-generator-exponential-distribution
		number=RandomGenerator.nextFloat();
		return (float) (Math.log(1-number)*-(mean)); 
	}
	
	static int count_LinkedList(LinkedList List)
	{
		LinkedList temp=null;
		int count=0;
		if(List.next==null)
			return 0;
		else
		{
			temp=List.next;
			while(temp!=null)
			{
				count+=1;
				temp=temp.next;
			}
			return count;
		}
	}
	static void print_LinkedList(LinkedList List)
	{
		LinkedList temp=null;
		if(List.next==null)
			System.out.print("|");
		else
		{
			temp=List.next;
			while(temp!=null)
			{
				System.out.print( temp.exit_time +"("+temp.delay+") ");
				temp=temp.next;
			}
			System.out.print("|");
		}
	}
	static void delete_Node_LinkedList(LinkedList List)
	{
		//LinkedList temp;
		if(List.next==null)
		{
			System.out.println("FAULT: The List is Empty");
			if(enableDebug)
				print_LinkedList(List);
			
		}
		else
		{
			//temp=List.next;
			List.next=List.next.next;
		}
	}
	static void add_Node_Client_Queue(float ClientServerRN, float f, float start_time, LinkedList List)
	{
		LinkedList temp = new LinkedList();
		temp.delay=ClientServerRN;
		temp.exit_time=f;
		temp.start_time=start_time;
		if(List.next==null)//List is Empty
		{
			if(enableDebug)
				System.out.println("anll2");
			List.next=temp;
			if(enableDebug)
				print_LinkedList(List);
		}
		
		else	// Inserting in the middle of the linked list
		{
			if(enableDebug)
				System.out.println("anll4");
			LinkedList traversal2=null;
			traversal2=List;
			if(enableDebug)
				print_LinkedList(List);
			while(traversal2.next!=null )
				traversal2=traversal2.next;
		    traversal2.next=temp;
		}
	}
	
	static void add_Node_LinkedList(float infiniteServerRN, float f,float start_time, LinkedList List)
	{
		if(enableDebug)
			System.out.println("anll1 "+ infiniteServerRN +" "+f);
		LinkedList temp = new LinkedList();
		temp.delay=infiniteServerRN;
		temp.exit_time=f;
		temp.start_time=start_time;
		if(List.next==null)//List is Empty
		{
			if(enableDebug)
				System.out.println("anll2");
			List.next=temp;
			if(enableDebug)
				print_LinkedList(List);
		}
		
		else	// Inserting in the middle of the linked list
		{
			if(enableDebug)
				System.out.println("anll4");
			LinkedList traversal2=null;
			LinkedList traversal1=null;
			traversal2=List;
			if(enableDebug)
				print_LinkedList(List);
			while(traversal2!=null && traversal2.exit_time<f )
				traversal2=traversal2.next;
			traversal1=List;
			if(traversal2==null)
			{
				while(traversal1.next!=null)
					traversal1=traversal1.next;
				traversal1.next=temp;
			}
			else
			{
				while(traversal1.next!=traversal2)
					traversal1=traversal1.next;
				temp.next=traversal1.next;
				traversal1.next=temp;	
			}
		}
	}
	
	static float find_max(LinkedList List)
	{
		LinkedList temp=List;
		float max_element =-1;
		if(temp.next==null)
		{
			//System.out.println();
			//System.out.println("::::||||||||||||||||||||::::::::::: "+ temp.exit_time);
			return CLK;
		}
		else
		{
			System.out.println();
			//System.out.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
			//print_LinkedList(Client_queue);
			while(temp!=null)
			{
				
				//System.out.println("::::::::::::::: "+ temp.exit_time);
				if(temp.exit_time>max_element)
					max_element=temp.exit_time;
				temp=temp.next;
			}
			//System.out.println();
			//System.out.print("---------------------------------------------- "+max_element );
			return max_element;
		}
	}
	
	static void event1()
	{
			System.out.println();
			float infiniteServerRN=0;
			CLK=nextDeparture;
			System.out.print(" "+CLK+"|");
			System.out.print(" 1|");
			infiniteServerRN=randomgenerator(InfiniteServerRate);			
			//if(enableDebug)
			{
				temp_count_isq++;
				mean_isq+=infiniteServerRN;
			}
			add_Node_LinkedList(infiniteServerRN,(CLK+infiniteServerRN),CLK-delay_server_queue,Infinite_server_queue);
			if(flag==true)
				delay_server_queue=randomgenerator(D_H);
			else
				delay_server_queue=randomgenerator(D_L);
			//if(enableDebug)
			{
				temp_count_sq++;
				mean_sq+=delay_server_queue;
			}
			nextDeparture=CLK+delay_server_queue;
			
			//Assignment 3
			N_Deps++;
			if(slowDebug)
			{
				try {
				    Thread.sleep(1000);
				} catch(InterruptedException ex) {
				    Thread.currentThread().interrupt();
				}
			}
		if(flag==true)
								System.out.print(" H|" );
		else
			System.out.print(" L|" );
		System.out.print(" "+nextDeparture+"|");
		print_LinkedList(Infinite_server_queue);
		if(Client_queue.next!=null)
			System.out.print(" "+Client_queue.next.exit_time+"|");
		else
			System.out.print("|");
		System.out.print(" "+count_LinkedList(Client_queue)+"|");
		System.out.print("|");
	}
	static void event2()
	{
		float clientServerRN,start_time;
		System.out.println();
		CLK=Infinite_server_queue.next.exit_time;
		System.out.print(" "+CLK+"|");
		//Event 2
		System.out.print(" 2|");
		if(flag==true)
			System.out.print(" H|" );
		else
			System.out.print(" L|" );
		System.out.print(" "+nextDeparture+"|");
		if(Infinite_server_queue.next==null)
			System.out.print(" |");
		else
		{
			if(enableDebug)
			{
				System.out.print(" DEBUG: ");
				print_LinkedList(Infinite_server_queue);
			}
				
			if(enableDebug)
			{
				System.out.print("DEBUG");
				print_LinkedList(Infinite_server_queue);
			}
			start_time=Infinite_server_queue.next.start_time;
			//System.out.println(">>>>>>>>>>>>>>>>>Start_time event 2 "+start_time);
			delete_Node_LinkedList(Infinite_server_queue);
			clientServerRN = randomgenerator(ClientQueueRate);
			//if(enableDebug)
			{
				temp_count_client_queue++;
				mean_client_queue+=clientServerRN;
			}
			//Assignment 3
			add_Node_Client_Queue(clientServerRN,(clientServerRN+find_max(Client_queue)),start_time,Client_queue);
			//System.out.println();
			//System.out.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
			//print_LinkedList(Client_queue);
			//System.out.println();
				
			if(enableDebug)
			{
				System.out.print("E2: Client_queue: ");
				print_LinkedList(Client_queue);
			}
			
			if(count_LinkedList(Client_queue)<=T_L)
				flag=true;
			else if(count_LinkedList(Client_queue)>T_H)
				flag=false;
		}
		if(slowDebug)
		{
			try {
			    Thread.sleep(1000);
			} catch(InterruptedException ex) {
			    Thread.currentThread().interrupt();
			}
		}
		print_LinkedList(Infinite_server_queue);
		if(Client_queue.next!=null)
			System.out.print(" "+Client_queue.next.exit_time+"|");
		else
			System.out.print("|");
		//if(enableDebug)
		{
			temp_count_no_cust++;
			count_no_cust+=count_LinkedList(Client_queue);
		}
		System.out.print(" "+count_LinkedList(Client_queue)+"|");
		if(flag==true)
			System.out.print(" UP|" );
		else
			System.out.print(" DOWN|" );
	
	}
	static void event3()
	{
		//Event 3
		System.out.println();
		CLK=Client_queue.next.exit_time;
		System.out.print(" "+CLK+"|");
		System.out.print(" 3|");
		if(flag==true)
			System.out.print(" H|" );
		else
			System.out.print(" L|" );
		System.out.print(" "+nextDeparture+"|");
		LinkedList temp;
		if(Client_queue.next==null)
			System.out.print(" |");
		else
		{
			temp=Client_queue.next;
			if(enableDebug)
			{
				System.out.print("E3: After :Client_queue: ");
				print_LinkedList(Client_queue);
			}
			
			if(temp.exit_time<=CLK)
			{
				//Assignment 3
				//System.out.println("N = "+N+" Recieved packet = "+received_packet);
				//System.out.println(">>>>>>>>>>>>>>>>>>> Start Time: "+Client_queue.next.start_time+" e2e delay: "+(temp.exit_time-temp.start_time));
				end_to_end_delay[received_packet]=temp.exit_time-temp.start_time;
				received_packet+=1;
				delete_Node_LinkedList(Client_queue);
				N--;
			}
			if(enableDebug)
			{
				System.out.print("E3: After :Client_queue: ");
				print_LinkedList(Client_queue);
			}
		}
		if(slowDebug)
		{
			try {
			    Thread.sleep(1000);
			} catch(InterruptedException ex) {
			    Thread.currentThread().interrupt();
			}
		}
		print_LinkedList(Infinite_server_queue);
		if(Client_queue.next!=null)
			System.out.print(" "+Client_queue.next.exit_time+"|");
		else
			System.out.print(" |");
		System.out.print(" "+count_LinkedList(Client_queue)+"|");
		System.out.print("|");
	}
	static void randomnumbergenerator_1000()
	 {
		 
			int j=0;
			float a[]=new float[1000];
			FileOutputStream fos = null;
			try {
				fos = new FileOutputStream("random_5000.txt");
			} catch (FileNotFoundException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			PrintWriter dos = new PrintWriter(fos);
			while(j<1000)
			{
				double num1 = randomgenerator((float)10); //generate a random number
				double num2 = randomgenerator((float)1.5);
				System.out.println(j);
				a[j]=(float) (num1+num2);
				j=j+1;
				
			}
			Arrays.sort(a); 
			for(int i=0; 1000 > i; i++)
			{
				dos.println(a[i]); //write the number to the file 
			}
			
			dos.close();
	 }
	static void loop_events(int n)
	{
		N=n;
		while(N>0)
		{
				
				if(Infinite_server_queue.next==null && Client_queue.next==null)
				{
					event1();
				}
				else
				{
					if(Infinite_server_queue.next!=null && Client_queue.next!=null && nextDeparture<=Infinite_server_queue.next.exit_time && nextDeparture<=Client_queue.next.exit_time)
						event1();
					if(Infinite_server_queue.next==null && Client_queue.next!=null &&  nextDeparture<=Client_queue.next.exit_time)
						event1();
					if(Infinite_server_queue.next!=null && Client_queue.next==null && nextDeparture<=Infinite_server_queue.next.exit_time)
						event1();
					if(Infinite_server_queue.next!=null && Client_queue.next!=null && Infinite_server_queue.next.exit_time<nextDeparture && Infinite_server_queue.next.exit_time<=Client_queue.next.exit_time)
						event2();
					if(Infinite_server_queue.next!=null && Client_queue.next==null && Infinite_server_queue.next.exit_time<nextDeparture)
					{
						event2();
					}
					if(Infinite_server_queue.next==null && Client_queue.next!=null && Client_queue.next.exit_time<nextDeparture)
					{
						event3();
					}
					if(Infinite_server_queue.next!=null && Client_queue.next!=null && Client_queue.next.exit_time<nextDeparture && Client_queue.next.exit_time<Infinite_server_queue.next.exit_time)
					{
						event3();
					}
				}
		}
	}
	
	public static void main(String args[]) throws NumberFormatException, IOException
	{
		//Setting the seed for the random number generator
		RandomGenerator= new Random();
		RandomGenerator.setSeed(0);
		
		
		Infinite_server_queue=new LinkedList();
		Client_queue=new LinkedList();
		
		
		
		//Accept input from user
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Enter the Number of packets that departed from the client queue :");
		N_total= Integer.parseInt(br.readLine());
		System.out.println("Enter the Batch size :");
		batch_size = Integer.parseInt(br.readLine());
		System.out.println("Time to transmit a packet from the server at a high rate");
		D_H= Integer.parseInt(br.readLine());
		System.out.println("Time to transmit a packet from the server at a low rate");
		D_L= Integer.parseInt(br.readLine());
		System.out.println("Mean service time in the server queue");
		InfiniteServerRate=Float.parseFloat(br.readLine());
		System.out.println("Mean service time in the client queue");
		ClientQueueRate=Float.parseFloat(br.readLine());
		System.out.println("High Threshold level in the server queue");
		T_H=Integer.parseInt(br.readLine());
		System.out.println("Low Threshold level in the server queue");
		T_L=Integer.parseInt(br.readLine());
		
		nextDeparture=D_H;
		delay_server_queue=D_H;
		end_to_end_delay = new float[batch_size];
		
	
		
		if(enableDebug)
		{
			print_LinkedList(Infinite_server_queue);
			System.out.println(count_LinkedList(Infinite_server_queue));	
			System.out.println(" "+N+" "+D_H+" "+D_L+" "+T_L+" "+T_H);
			System.out.println(" "+InfiniteServerRate+" "+ClientQueueRate);
		}
		
		System.out.println("0||H|1||||");
		
		loop_events(500);		
		received_packet=0;
		num_of_batches=N_total/batch_size;
		
		for(int index=0;index<num_of_batches;index++)
		{
			loop_events(batch_size);
			 Arrays.sort(end_to_end_delay); 
			 batch_95[index]=end_to_end_delay[(int) Math.ceil(received_packet*0.95)];
			 float mean_temp=0;
			 for(int x=0;x<received_packet;x++)
				 mean_temp+=end_to_end_delay[x];
			 mean[index]=mean_temp/received_packet;
			 received_packet=0;
			 if(slowDebug)
				{
					try {
					    Thread.sleep(10000);
					} catch(InterruptedException ex) {
					    Thread.currentThread().interrupt();
					}
				}
		}
		
		System.out.println();
		System.out.println("The End to End Delay is :: ");
		
		//Assignment 3: Mean
		float sum_mean_batch_95=0;
		float sum_mean=0;
		for(int i=0;i<num_of_batches;i++)
		{
			sum_mean_batch_95+=batch_95[i];
			sum_mean+=mean[i];
		}
		T_mean = sum_mean_batch_95/num_of_batches;
		e2e_mean=sum_mean/num_of_batches;
		
		
		//Assignment 3: Mean
		float sum_sd=0;
		for(int i=0;i<num_of_batches;i++)
			sum_sd+=Math.pow((T_mean - batch_95[i]), 2);
		T_sd = (float) Math.sqrt(sum_sd/(num_of_batches-1));
		
		System.out.println("Mean: "+T_mean+" SD: "+ T_sd + " Mean e2e : "+e2e_mean);
		System.out.println((T_mean -(1.96*T_sd)/Math.sqrt(num_of_batches))+" , "+(T_mean +(1.96*T_sd)/Math.sqrt(num_of_batches)));		
	}
	
=======
import java.util.Random;
import java.util.Arrays;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
public class Assignment
{
	private static final boolean enableDebug = false;
	private static final boolean slowDebug = false;
	
	//Queues
	static LinkedList Infinite_server_queue;
	static LinkedList Client_queue;
	//Assignment 3:
	static float[] end_to_end_delay;
	static float[] batch_95 = new float[30];
	static float[] mean = new float[30];
	static int received_packet=0;
	static int N_Deps=0;
	static int batch_size=0;
	static int N_total=0;
	static float T_mean=0;
	static float T_sd=0;
	static int num_of_batches=0;
	static float e2e_mean=0;
	static float delay_server_queue=0;
	
	//debug
	static int temp_count_client_queue=0;
	static float mean_client_queue=0;
	static int temp_count_isq=0;
	static float mean_isq=0;
	static int temp_count_sq=0;
	static float mean_sq=0;
	static int temp_count_no_cust=0;
	static int count_no_cust=0;
	
	
	
	static Random RandomGenerator;
	int Clock=0;
	//
	static boolean flag=true;
	static float nextDeparture=0;
	static float CLK=0;
	
	//Basic User Inputs
	static float N,D_H,D_L;
	static int T_L,T_H;
	static float InfiniteServerRate,ClientQueueRate;
	
	static float randomgenerator(float mean)
	{
		float number;
		//Exponential Distribution: http://stackoverflow.com/questions/2106503/pseudorandom-number-generator-exponential-distribution
		number=RandomGenerator.nextFloat();
		return (float) (Math.log(1-number)*-(mean)); 
	}
	
	static int count_LinkedList(LinkedList List)
	{
		LinkedList temp=null;
		int count=0;
		if(List.next==null)
			return 0;
		else
		{
			temp=List.next;
			while(temp!=null)
			{
				count+=1;
				temp=temp.next;
			}
			return count;
		}
	}
	static void print_LinkedList(LinkedList List)
	{
		LinkedList temp=null;
		if(List.next==null)
			System.out.print("|");
		else
		{
			temp=List.next;
			while(temp!=null)
			{
				System.out.print( temp.exit_time +"("+temp.delay+") ");
				temp=temp.next;
			}
			System.out.print("|");
		}
	}
	static void delete_Node_LinkedList(LinkedList List)
	{
		//LinkedList temp;
		if(List.next==null)
		{
			System.out.println("FAULT: The List is Empty");
			if(enableDebug)
				print_LinkedList(List);
			
		}
		else
		{
			//temp=List.next;
			List.next=List.next.next;
		}
	}
	static void add_Node_Client_Queue(float ClientServerRN, float f, float start_time, LinkedList List)
	{
		LinkedList temp = new LinkedList();
		temp.delay=ClientServerRN;
		temp.exit_time=f;
		temp.start_time=start_time;
		if(List.next==null)//List is Empty
		{
			if(enableDebug)
				System.out.println("anll2");
			List.next=temp;
			if(enableDebug)
				print_LinkedList(List);
		}
		
		else	// Inserting in the middle of the linked list
		{
			if(enableDebug)
				System.out.println("anll4");
			LinkedList traversal2=null;
			traversal2=List;
			if(enableDebug)
				print_LinkedList(List);
			while(traversal2.next!=null )
				traversal2=traversal2.next;
		    traversal2.next=temp;
		}
	}
	
	static void add_Node_LinkedList(float infiniteServerRN, float f,float start_time, LinkedList List)
	{
		if(enableDebug)
			System.out.println("anll1 "+ infiniteServerRN +" "+f);
		LinkedList temp = new LinkedList();
		temp.delay=infiniteServerRN;
		temp.exit_time=f;
		temp.start_time=start_time;
		if(List.next==null)//List is Empty
		{
			if(enableDebug)
				System.out.println("anll2");
			List.next=temp;
			if(enableDebug)
				print_LinkedList(List);
		}
		
		else	// Inserting in the middle of the linked list
		{
			if(enableDebug)
				System.out.println("anll4");
			LinkedList traversal2=null;
			LinkedList traversal1=null;
			traversal2=List;
			if(enableDebug)
				print_LinkedList(List);
			while(traversal2!=null && traversal2.exit_time<f )
				traversal2=traversal2.next;
			traversal1=List;
			if(traversal2==null)
			{
				while(traversal1.next!=null)
					traversal1=traversal1.next;
				traversal1.next=temp;
			}
			else
			{
				while(traversal1.next!=traversal2)
					traversal1=traversal1.next;
				temp.next=traversal1.next;
				traversal1.next=temp;	
			}
		}
	}
	
	static float find_max(LinkedList List)
	{
		LinkedList temp=List;
		float max_element =-1;
		if(temp.next==null)
		{
			//System.out.println();
			//System.out.println("::::||||||||||||||||||||::::::::::: "+ temp.exit_time);
			return CLK;
		}
		else
		{
			System.out.println();
			//System.out.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
			//print_LinkedList(Client_queue);
			while(temp!=null)
			{
				
				//System.out.println("::::::::::::::: "+ temp.exit_time);
				if(temp.exit_time>max_element)
					max_element=temp.exit_time;
				temp=temp.next;
			}
			//System.out.println();
			//System.out.print("---------------------------------------------- "+max_element );
			return max_element;
		}
	}
	
	static void event1()
	{
			System.out.println();
			float infiniteServerRN=0;
			CLK=nextDeparture;
			System.out.print(" "+CLK+"|");
			System.out.print(" 1|");
			infiniteServerRN=randomgenerator(InfiniteServerRate);			
			//if(enableDebug)
			{
				temp_count_isq++;
				mean_isq+=infiniteServerRN;
			}
			add_Node_LinkedList(infiniteServerRN,(CLK+infiniteServerRN),CLK-delay_server_queue,Infinite_server_queue);
			if(flag==true)
				delay_server_queue=randomgenerator(D_H);
			else
				delay_server_queue=randomgenerator(D_L);
			//if(enableDebug)
			{
				temp_count_sq++;
				mean_sq+=delay_server_queue;
			}
			nextDeparture=CLK+delay_server_queue;
			
			//Assignment 3
			N_Deps++;
			if(slowDebug)
			{
				try {
				    Thread.sleep(1000);
				} catch(InterruptedException ex) {
				    Thread.currentThread().interrupt();
				}
			}
		if(flag==true)
								System.out.print(" H|" );
		else
			System.out.print(" L|" );
		System.out.print(" "+nextDeparture+"|");
		print_LinkedList(Infinite_server_queue);
		if(Client_queue.next!=null)
			System.out.print(" "+Client_queue.next.exit_time+"|");
		else
			System.out.print("|");
		System.out.print(" "+count_LinkedList(Client_queue)+"|");
		System.out.print("|");
	}
	static void event2()
	{
		float clientServerRN,start_time;
		System.out.println();
		CLK=Infinite_server_queue.next.exit_time;
		System.out.print(" "+CLK+"|");
		//Event 2
		System.out.print(" 2|");
		if(flag==true)
			System.out.print(" H|" );
		else
			System.out.print(" L|" );
		System.out.print(" "+nextDeparture+"|");
		if(Infinite_server_queue.next==null)
			System.out.print(" |");
		else
		{
			if(enableDebug)
			{
				System.out.print(" DEBUG: ");
				print_LinkedList(Infinite_server_queue);
			}
				
			if(enableDebug)
			{
				System.out.print("DEBUG");
				print_LinkedList(Infinite_server_queue);
			}
			start_time=Infinite_server_queue.next.start_time;
			//System.out.println(">>>>>>>>>>>>>>>>>Start_time event 2 "+start_time);
			delete_Node_LinkedList(Infinite_server_queue);
			clientServerRN = randomgenerator(ClientQueueRate);
			//if(enableDebug)
			{
				temp_count_client_queue++;
				mean_client_queue+=clientServerRN;
			}
			//Assignment 3
			add_Node_Client_Queue(clientServerRN,(clientServerRN+find_max(Client_queue)),start_time,Client_queue);
			//System.out.println();
			//System.out.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
			//print_LinkedList(Client_queue);
			//System.out.println();
				
			if(enableDebug)
			{
				System.out.print("E2: Client_queue: ");
				print_LinkedList(Client_queue);
			}
			
			if(count_LinkedList(Client_queue)<=T_L)
				flag=true;
			else if(count_LinkedList(Client_queue)>T_H)
				flag=false;
		}
		if(slowDebug)
		{
			try {
			    Thread.sleep(1000);
			} catch(InterruptedException ex) {
			    Thread.currentThread().interrupt();
			}
		}
		print_LinkedList(Infinite_server_queue);
		if(Client_queue.next!=null)
			System.out.print(" "+Client_queue.next.exit_time+"|");
		else
			System.out.print("|");
		//if(enableDebug)
		{
			temp_count_no_cust++;
			count_no_cust+=count_LinkedList(Client_queue);
		}
		System.out.print(" "+count_LinkedList(Client_queue)+"|");
		if(flag==true)
			System.out.print(" UP|" );
		else
			System.out.print(" DOWN|" );
	
	}
	static void event3()
	{
		//Event 3
		System.out.println();
		CLK=Client_queue.next.exit_time;
		System.out.print(" "+CLK+"|");
		System.out.print(" 3|");
		if(flag==true)
			System.out.print(" H|" );
		else
			System.out.print(" L|" );
		System.out.print(" "+nextDeparture+"|");
		LinkedList temp;
		if(Client_queue.next==null)
			System.out.print(" |");
		else
		{
			temp=Client_queue.next;
			if(enableDebug)
			{
				System.out.print("E3: After :Client_queue: ");
				print_LinkedList(Client_queue);
			}
			
			if(temp.exit_time<=CLK)
			{
				//Assignment 3
				//System.out.println("N = "+N+" Recieved packet = "+received_packet);
				//System.out.println(">>>>>>>>>>>>>>>>>>> Start Time: "+Client_queue.next.start_time+" e2e delay: "+(temp.exit_time-temp.start_time));
				end_to_end_delay[received_packet]=temp.exit_time-temp.start_time;
				received_packet+=1;
				delete_Node_LinkedList(Client_queue);
				N--;
			}
			if(enableDebug)
			{
				System.out.print("E3: After :Client_queue: ");
				print_LinkedList(Client_queue);
			}
		}
		if(slowDebug)
		{
			try {
			    Thread.sleep(1000);
			} catch(InterruptedException ex) {
			    Thread.currentThread().interrupt();
			}
		}
		print_LinkedList(Infinite_server_queue);
		if(Client_queue.next!=null)
			System.out.print(" "+Client_queue.next.exit_time+"|");
		else
			System.out.print(" |");
		System.out.print(" "+count_LinkedList(Client_queue)+"|");
		System.out.print("|");
	}
	static void randomnumbergenerator_1000()
	 {
		 
			int j=0;
			float a[]=new float[1000];
			FileOutputStream fos = null;
			try {
				fos = new FileOutputStream("random_5000.txt");
			} catch (FileNotFoundException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			PrintWriter dos = new PrintWriter(fos);
			while(j<1000)
			{
				double num1 = randomgenerator((float)10); //generate a random number
				double num2 = randomgenerator((float)1.5);
				System.out.println(j);
				a[j]=(float) (num1+num2);
				j=j+1;
				
			}
			Arrays.sort(a); 
			for(int i=0; 1000 > i; i++)
			{
				dos.println(a[i]); //write the number to the file 
			}
			
			dos.close();
	 }
	static void loop_events(int n)
	{
		N=n;
		while(N>0)
		{
				
				if(Infinite_server_queue.next==null && Client_queue.next==null)
				{
					event1();
				}
				else
				{
					if(Infinite_server_queue.next!=null && Client_queue.next!=null && nextDeparture<=Infinite_server_queue.next.exit_time && nextDeparture<=Client_queue.next.exit_time)
						event1();
					if(Infinite_server_queue.next==null && Client_queue.next!=null &&  nextDeparture<=Client_queue.next.exit_time)
						event1();
					if(Infinite_server_queue.next!=null && Client_queue.next==null && nextDeparture<=Infinite_server_queue.next.exit_time)
						event1();
					if(Infinite_server_queue.next!=null && Client_queue.next!=null && Infinite_server_queue.next.exit_time<nextDeparture && Infinite_server_queue.next.exit_time<=Client_queue.next.exit_time)
						event2();
					if(Infinite_server_queue.next!=null && Client_queue.next==null && Infinite_server_queue.next.exit_time<nextDeparture)
					{
						event2();
					}
					if(Infinite_server_queue.next==null && Client_queue.next!=null && Client_queue.next.exit_time<nextDeparture)
					{
						event3();
					}
					if(Infinite_server_queue.next!=null && Client_queue.next!=null && Client_queue.next.exit_time<nextDeparture && Client_queue.next.exit_time<Infinite_server_queue.next.exit_time)
					{
						event3();
					}
				}
		}
	}
	
	public static void main(String args[]) throws NumberFormatException, IOException
	{
		//Setting the seed for the random number generator
		RandomGenerator= new Random();
		RandomGenerator.setSeed(0);
		
		
		Infinite_server_queue=new LinkedList();
		Client_queue=new LinkedList();
		
		
		
		//Accept input from user
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Enter the Number of packets that departed from the client queue :");
		N_total= Integer.parseInt(br.readLine());
		System.out.println("Enter the Batch size :");
		batch_size = Integer.parseInt(br.readLine());
		System.out.println("Time to transmit a packet from the server at a high rate");
		D_H= Integer.parseInt(br.readLine());
		System.out.println("Time to transmit a packet from the server at a low rate");
		D_L= Integer.parseInt(br.readLine());
		System.out.println("Mean service time in the server queue");
		InfiniteServerRate=Float.parseFloat(br.readLine());
		System.out.println("Mean service time in the client queue");
		ClientQueueRate=Float.parseFloat(br.readLine());
		System.out.println("High Threshold level in the server queue");
		T_H=Integer.parseInt(br.readLine());
		System.out.println("Low Threshold level in the server queue");
		T_L=Integer.parseInt(br.readLine());
		
		nextDeparture=D_H;
		delay_server_queue=D_H;
		end_to_end_delay = new float[batch_size];
		
	
		
		if(enableDebug)
		{
			print_LinkedList(Infinite_server_queue);
			System.out.println(count_LinkedList(Infinite_server_queue));	
			System.out.println(" "+N+" "+D_H+" "+D_L+" "+T_L+" "+T_H);
			System.out.println(" "+InfiniteServerRate+" "+ClientQueueRate);
		}
		
		System.out.println("0||H|1||||");
		
		loop_events(500);		
		received_packet=0;
		num_of_batches=N_total/batch_size;
		
		for(int index=0;index<num_of_batches;index++)
		{
			loop_events(batch_size);
			 Arrays.sort(end_to_end_delay); 
			 batch_95[index]=end_to_end_delay[(int) Math.ceil(received_packet*0.95)];
			 float mean_temp=0;
			 for(int x=0;x<received_packet;x++)
				 mean_temp+=end_to_end_delay[x];
			 mean[index]=mean_temp/received_packet;
			 received_packet=0;
			 if(slowDebug)
				{
					try {
					    Thread.sleep(10000);
					} catch(InterruptedException ex) {
					    Thread.currentThread().interrupt();
					}
				}
		}
		
		System.out.println();
		System.out.println("The End to End Delay is :: ");
		
		//Assignment 3: Mean
		float sum_mean_batch_95=0;
		float sum_mean=0;
		for(int i=0;i<num_of_batches;i++)
		{
			sum_mean_batch_95+=batch_95[i];
			sum_mean+=mean[i];
		}
		T_mean = sum_mean_batch_95/num_of_batches;
		e2e_mean=sum_mean/num_of_batches;
		
		
		//Assignment 3: Mean
		float sum_sd=0;
		for(int i=0;i<num_of_batches;i++)
			sum_sd+=Math.pow((T_mean - batch_95[i]), 2);
		T_sd = (float) Math.sqrt(sum_sd/(num_of_batches-1));
		
		System.out.println("Mean: "+T_mean+" SD: "+ T_sd + " Mean e2e : "+e2e_mean);
		System.out.println((T_mean -(1.96*T_sd)/Math.sqrt(num_of_batches))+" , "+(T_mean +(1.96*T_sd)/Math.sqrt(num_of_batches)));		
	}
	
>>>>>>> 3e3b2787693bca199252176ce6bdefb3836c8875
}