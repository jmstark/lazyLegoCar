package com.example.helloworld;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class MainActivity extends Activity {

	Button mainButton;
	Button leftButton; 
    Button rightButton; 
    Button driveButton; 
    Button backButton;
    Animation buttonEffect;
    Drawable d;
    public static final int ServerPort = 4359;
    String ServerIP;
    
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
		          seekBar.setMax(100);
		          
		          if (ServerIP != null)
		          	reading.setText("ServerIP: "+ServerIP);
		          
		          try {
		  			Socket socket = new Socket(ServerIP, ServerPort);
		  			if (socket.isConnected())
		  				reading.setText("Connection succeeds!");
		  			else
		  				reading.setText("Connection fails!");
		  	        socket.close();
		  	       
		  		} catch (UnknownHostException e) {
		  		        
		  		        e.printStackTrace();
		  		} catch (IOException e) {
		  		        
		  		        e.printStackTrace();
		  		}

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
		  			   try {
		  					Socket socket = new Socket(ServerIP, ServerPort);
		  			        DataOutputStream DOS = new DataOutputStream(socket.getOutputStream());
		  			        DOS.writeUTF("Speed "+progress+ " cm/s");
		  			        socket.close();
		  			       
		  				} catch (UnknownHostException e) {
		  				        
		  				        e.printStackTrace();
		  				} catch (IOException e) {
		  				        
		  				        e.printStackTrace();
		  				}
		  		   }
		  		});
		          
		          buttonEffect = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.button_effect);
		          
		          mainButton = (Button) findViewById(R.id.button_main);
		          mainButton.setOnClickListener(new OnClickListener(){
		          	@Override
		              public void onClick(View v){
		          		mainButton.startAnimation(buttonEffect);
		          		try {
		  					Socket socket = new Socket(ServerIP, ServerPort);
		  			        DataOutputStream DOS = new DataOutputStream(socket.getOutputStream());
		  			        DOS.writeUTF("EXIT");
		  			        socket.close();
		  			       
		  				} catch (UnknownHostException e) {
		  				        
		  				        e.printStackTrace();
		  				} catch (IOException e) {
		  				        
		  				        e.printStackTrace();
		  				}
		                  finish();
		          	}
		          });
		          	

		          leftButton = (Button) findViewById(R.id.button_left);
		          rightButton = (Button) findViewById(R.id.button_right);
		          driveButton = (Button) findViewById(R.id.button_forward);
		          backButton = (Button) findViewById(R.id.button_backward);
		          d = (Drawable) mainButton.getBackground();
		          
		          leftButton.setOnTouchListener(new View.OnTouchListener() {
		  			@Override
		  			public boolean onTouch(View v, MotionEvent event) {
		  				
		  				if(event.getAction() == MotionEvent.ACTION_DOWN || event.getAction() == MotionEvent.ACTION_MOVE) {
		  					leftButton.setBackgroundColor(Color.CYAN);
		  					try {
		  						Socket socket = new Socket(ServerIP, ServerPort);
		  				        DataOutputStream DOS = new DataOutputStream(socket.getOutputStream());
		  				        DOS.writeUTF("LEFT");
		  				        socket.close();
		  				       
		  					} catch (UnknownHostException e) {
		  					        
		  					        e.printStackTrace();
		  					} catch (IOException e) {
		  					        
		  					        e.printStackTrace();
		  					}

		  				}
		  				else if(event.getAction() == MotionEvent.ACTION_UP)
		  					leftButton.setBackgroundDrawable(d);
		  				return true;
		  			}
		  		});
		          
		          
		          rightButton.setOnTouchListener(new View.OnTouchListener() {
		  			
		  			@Override
		  			public boolean onTouch(View v, MotionEvent event) {
		  				
		  				if(event.getAction() == MotionEvent.ACTION_DOWN || event.getAction() == MotionEvent.ACTION_MOVE) {
		  					rightButton.setBackgroundColor(Color.GREEN);
		  					try {
		  						Socket socket = new Socket(ServerIP, ServerPort);
		  				        DataOutputStream DOS = new DataOutputStream(socket.getOutputStream());
		  				        DOS.writeUTF("RIGHT");
		  				        socket.close();
		  				       
		  					} catch (UnknownHostException e) {
		  					        
		  					        e.printStackTrace();
		  					} catch (IOException e) {
		  					        
		  					        e.printStackTrace();
		  					}
		  				}
		  				else if(event.getAction() == MotionEvent.ACTION_UP)
		  					rightButton.setBackgroundDrawable(d);
		  				return true;
		  			}
		  		});
		          
		          driveButton.setOnTouchListener(new View.OnTouchListener() {
		  			
		  			@Override
		  			public boolean onTouch(View v, MotionEvent event) {
		  				
		  				if(event.getAction() == MotionEvent.ACTION_DOWN || event.getAction() == MotionEvent.ACTION_MOVE) {
		  					driveButton.setBackgroundColor(Color.BLUE);
		  					try {
		  						Socket socket = new Socket(ServerIP, ServerPort);
		  				        DataOutputStream DOS = new DataOutputStream(socket.getOutputStream());
		  				        DOS.writeUTF("DRIVE");
		  				        socket.close();
		  				       
		  					} catch (UnknownHostException e) {
		  					        
		  					        e.printStackTrace();
		  					} catch (IOException e) {
		  					        
		  					        e.printStackTrace();
		  					}
		  				}
		  				else if(event.getAction() == MotionEvent.ACTION_UP)
		  					driveButton.setBackgroundDrawable(d);
		  				return true;
		  			}
		  		});
		          
		          backButton.setOnTouchListener(new View.OnTouchListener() {
		  			
		  			@Override
		  			public boolean onTouch(View v, MotionEvent event) {
		  				
		  				if(event.getAction() == MotionEvent.ACTION_DOWN || event.getAction() == MotionEvent.ACTION_MOVE) {
		  					backButton.setBackgroundColor(Color.YELLOW);
		  					try {
		  						Socket socket = new Socket(ServerIP, ServerPort);
		  				        DataOutputStream DOS = new DataOutputStream(socket.getOutputStream());
		  				        DOS.writeUTF("BACK");
		  				        socket.close();
		  				       
		  					} catch (UnknownHostException e) {
		  					        e.printStackTrace();
		  					} catch (IOException e) {
		  					        e.printStackTrace();
		  					}
		  				}
		  				else if(event.getAction() == MotionEvent.ACTION_UP)
		  					backButton.setBackgroundDrawable(d);
		  				return true;
		  			}
		  		});
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
        
        /*
        //sleep 2 seconds
        Handler handler = new Handler(); 
        handler.postDelayed(new Runnable() { 
             public void run() { 
             } 
        }, 2000);
        */	
	}
}



