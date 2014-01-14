package com.example.helloworld;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.drawable.ClipDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.LayerDrawable;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class MainActivity extends Activity {

	Button mainButton;
	Button leftButton; 
    Button rightButton; 
    Button driveButton; 
    Button backButton;
    Button straightButton;
    Animation buttonEffect;
    Drawable d;
    public static final int ServerPort = 4359;
    String ServerIP;
    Socket socket;
    PrintWriter printWriter;
    DataOutputStream DOS;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        AlertDialog.Builder alert = new AlertDialog.Builder(this);

        alert.setTitle("Type ServerIP! Port: " +ServerPort);

        // Set an EditText view to get user input 
        final EditText input = new EditText(this);
        alert.setView(input);
        
          
	        alert.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
	        public void onClick(DialogInterface dialog, int whichButton) {
			          ServerIP = input.getText().toString();
			          // Do something with value!
			          final TextView reading = (TextView) findViewById(R.id.reading);
			          SeekBar seekBar = (SeekBar)findViewById(R.id.seekbar);
			          seekBar.setMax(255);
			          
			          if (ServerIP != null)
			          	reading.setText("ServerIP: "+ServerIP);      
			         
			          try {
						  socket = new Socket(ServerIP, ServerPort);
						  printWriter = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
						  DOS = new DataOutputStream(socket.getOutputStream());
			        	  
						  if (socket.isConnected())
				  				reading.setText("Connection succeeds!");
				  			else
				  				reading.setText("Connection fails!");
	
			          seekBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			     
			  		   @Override
			  		   public void onStopTrackingTouch(SeekBar seekBar) {
			  		   }
			  		   
			  		   @Override
			  		   public void onStartTrackingTouch(SeekBar seekBar) {
			  		    
			  		   }
			  		   
			  		   @Override
			  		   public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
			  			   reading.setText("Speed "+progress+ " cm/s");
			  			   
			  			   if(progress <= 64){
			  				   seekBar.setBackgroundColor(Color.RED);
			  			   }else{
			  				  seekBar.setBackgroundColor(Color.GREEN);
			  				  //seekBar.setProgressDrawable(new ColorDrawable(Color.GREEN));
			  				  //setProgressBarColor(seekBar, Color.GREEN);
			  			   }
			  			 
			  			 try {
							DOS.writeUTF("Stop");
						} catch (IOException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
			  			 printWriter.print(Integer.toString(progress));//anscheinend printWriter nicht funktioniert??
			  			 printWriter.flush();
			  			   
			  		   }
			  		});
			          
			          buttonEffect = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.button_effect);
			          
			          mainButton = (Button) findViewById(R.id.button_main);
			          mainButton.setOnClickListener(new OnClickListener(){
			          	@Override
			              public void onClick(View v){
			          		mainButton.startAnimation(buttonEffect);
			          		try {
								DOS.writeUTF("Exit");
								socket.close();
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
			          		printWriter.print("Exit");
				  			printWriter.flush();
				  			
			                finish();
			          	}
			          });
			          	
			          straightButton = (Button) findViewById(R.id.button_straight);
			          leftButton = (Button) findViewById(R.id.button_left);
			          rightButton = (Button) findViewById(R.id.button_right);
			          driveButton = (Button) findViewById(R.id.button_forward);
			          backButton = (Button) findViewById(R.id.button_backward);
			          d = (Drawable) mainButton.getBackground();
			          
			          leftButton.setOnClickListener(new OnClickListener(){
			  			@Override
			  			public void onClick(View v){
			  					leftButton.setBackgroundColor(Color.CYAN);
			  					rightButton.setBackgroundDrawable(d);
			  					straightButton.setBackgroundDrawable(d);
			  					try {
									DOS.writeUTF("Left");
								} catch (IOException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								}
			  					printWriter.print("Left");
					  			printWriter.flush();
			  								  				}
			  		  });
			          
			          straightButton.setOnClickListener(new OnClickListener(){
				  			@Override
				  			public void onClick(View v){
				  					straightButton.setBackgroundColor(Color.RED);
				  					rightButton.setBackgroundDrawable(d);
				  					leftButton.setBackgroundDrawable(d);
				  					try {
										DOS.writeUTF("Straight");
									} catch (IOException e) {
										// TODO Auto-generated catch block
										e.printStackTrace();
									}
				  					printWriter.print("Straight");
						  			printWriter.flush();		
				  				}
				  		  });
			          
			          rightButton.setOnClickListener(new OnClickListener(){
			  			
			        	  @Override
				  			public void onClick(View v){
				  					rightButton.setBackgroundColor(Color.GREEN);
				  					leftButton.setBackgroundDrawable(d);
				  					straightButton.setBackgroundDrawable(d);
				  					try {
										DOS.writeUTF("Right");
									} catch (IOException e) {
										// TODO Auto-generated catch block
										e.printStackTrace();
									}
				  					printWriter.print("Right");
						  			printWriter.flush();
				  				
				  				}
				  		});
			          
			          driveButton.setOnTouchListener(new View.OnTouchListener() {
			  			
			  			@Override
			  			public boolean onTouch(View v, MotionEvent event) {		  				
			  				if(event.getAction() == MotionEvent.ACTION_DOWN) {
			  					driveButton.setBackgroundColor(Color.BLUE);
			  					try {
									DOS.writeUTF("Forwards");
								} catch (IOException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								}
			  					printWriter.print("Forwards");
					  			printWriter.flush();
			  						
			  				}
			  				else if(event.getAction() == MotionEvent.ACTION_UP)
			  				{
			  					driveButton.setBackgroundDrawable(d);
			  					try {
									DOS.writeUTF("Stop");
								} catch (IOException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								}
			  					printWriter.print("Stop");
					  			printWriter.flush();
			  					
			  				}
			  				return true;
			  			}
			  		});
			          
			          backButton.setOnTouchListener(new View.OnTouchListener() {
			  			
			  			@Override
			  			public boolean onTouch(View v, MotionEvent event) {
			  				if(event.getAction() == MotionEvent.ACTION_DOWN){
			  					backButton.setBackgroundColor(Color.YELLOW);
			  					try {
									DOS.writeUTF("Backwards");
								} catch (IOException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								}
			  					printWriter.print("Backwards");
					  			printWriter.flush();
			  					
			  				}
			  				else if(event.getAction() == MotionEvent.ACTION_UP)
			  				{
			  					backButton.setBackgroundDrawable(d);
			  					try {
									DOS.writeUTF("Stop");
								} catch (IOException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								}
			  					printWriter.print("Stop");
					  			printWriter.flush();
			  					
			  					/*
			  					try {
				  						Socket socket = new Socket(ServerIP, ServerPort);
				  				        DataOutputStream DOS = new DataOutputStream(socket.getOutputStream());			  				
				  				        DOS.writeUTF("Stop");
				  				        socket.close();  				        
			  					} catch (UnknownHostException e) {
			  					        e.printStackTrace();
			  					} catch (IOException e) {
			  					        e.printStackTrace();
			  					}*/
			  				}
			  				return true;
			  			}
			  		});
			          
		          
		          } catch (UnknownHostException e) {
		  	        e.printStackTrace();
		          } catch (IOException e) {    
		  	        e.printStackTrace();
		          }
	          }
	        });
	
	        alert.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
	          public void onClick(DialogInterface dialog, int whichButton) {
	            // Canceled.
	        	  dialog.cancel();
	        	  finish();
	          }
	        });
        
	        alert.show();
        
	}
	
	public void setProgressBarColor(ProgressBar progressBar, int newColor){ 
	    LayerDrawable ld = (LayerDrawable) progressBar.getProgressDrawable();
	    ClipDrawable d1 = (ClipDrawable) ld.findDrawableByLayerId(R.id.progressshape);
	    d1.setColorFilter(newColor, PorterDuff.Mode.SRC_IN);

	}
	
}



