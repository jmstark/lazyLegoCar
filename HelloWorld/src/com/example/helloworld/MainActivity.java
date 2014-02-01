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
    Button sendButton;
    Animation buttonEffect;
    Drawable d;
    public static final int ServerPort = 4359;
    public static final String ServerIP = "10.0.0.1";
    //public static final String ServerIP = "129.187.173.156";
    //String ServerIP;
    Socket socket;
    PrintWriter printWriter;
    DataOutputStream DOS;
    String xWert;
    String yWert;
    EditText xInput;
    EditText yInput;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        //AlertDialog.Builder alert = new AlertDialog.Builder(this);

        //alert.setTitle("Type ServerIP! Port: " +ServerPort);

        // Set an EditText view to get user input 
        //final EditText input = new EditText(this);
        //alert.setView(input);
        
          
        //alert.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
        //public void onClick(DialogInterface dialog, int whichButton) {
		          //ServerIP = input.getText().toString();
		          
		          final TextView reading = (TextView) findViewById(R.id.reading);
		          SeekBar seekBar = (SeekBar)findViewById(R.id.seekbar);
		          seekBar.setMax(255);
		          
		          xInput = (EditText) findViewById(R.id.xvalue);
		          yInput = (EditText) findViewById(R.id.yvalue);
		          
		          
		          if (ServerIP != null)
		          	reading.setText("ServerIP: "+ServerIP);      
		         
		          try {
					  socket = new Socket(ServerIP, ServerPort);
					  printWriter = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
					  //DOS = new DataOutputStream(socket.getOutputStream());
		        	  
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
		  			   }
		  			 
			  			 printWriter.print("Speed " + Integer.toString(progress));
			  			 printWriter.flush();
		  			   
		  		   }
		  		});
		          
		          buttonEffect = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.button_effect);
		          
		          mainButton = (Button) findViewById(R.id.button_main);
		          mainButton.setOnClickListener(new OnClickListener(){
		          	@Override
		              public void onClick(View v){
		          		mainButton.startAnimation(buttonEffect);
		          		printWriter.print("Exit");
			  			printWriter.flush();
			  			try {
							socket.close();
						} catch (IOException e) {
							e.printStackTrace();
						}
		                finish();
		          	}
		          });
		          	
		          straightButton = (Button) findViewById(R.id.button_straight);
		          leftButton = (Button) findViewById(R.id.button_left);
		          rightButton = (Button) findViewById(R.id.button_right);
		          driveButton = (Button) findViewById(R.id.button_forward);
		          backButton = (Button) findViewById(R.id.button_backward);
		          sendButton = (Button) findViewById(R.id.button_send);
		          
		          
		          d = (Drawable) mainButton.getBackground();
		          
		          sendButton.setOnClickListener(new OnClickListener(){
			  			@Override
			  			public void onClick(View v){
			  					sendButton.startAnimation(buttonEffect);
			  					
			  					xWert = xInput.getText().toString();
			  					yWert = yInput.getText().toString();
			  					String destination = "Destination " + (xWert.equals("") ? "0" : xWert) + " " +  (yWert.equals("") ? "0" : yWert);
			  					printWriter.print(destination);
					  			printWriter.flush();
			  			}
			  		  });
		          
		          leftButton.setOnClickListener(new OnClickListener(){
		  			@Override
		  			public void onClick(View v){
		  					leftButton.setBackgroundColor(Color.CYAN);
		  					rightButton.setBackgroundDrawable(d);
		  					straightButton.setBackgroundDrawable(d);
		  					
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
			  					
			  					printWriter.print("Right");
					  			printWriter.flush();
			  				
			  				}
			  		});
		          
		          driveButton.setOnTouchListener(new View.OnTouchListener() {
		  			
		  			@Override
		  			public boolean onTouch(View v, MotionEvent event) {		  				
		  				if(event.getAction() == MotionEvent.ACTION_DOWN) {
		  					driveButton.setBackgroundColor(Color.BLUE);
		  					
		  					printWriter.print("Forwards");
				  			printWriter.flush();
		  						
		  				}
		  				else if(event.getAction() == MotionEvent.ACTION_UP)
		  				{
		  					driveButton.setBackgroundDrawable(d);
		  					
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
		  					
		  					printWriter.print("Backwards");
				  			printWriter.flush();
		  					
		  				}
		  				else if(event.getAction() == MotionEvent.ACTION_UP)
		  				{
		  					backButton.setBackgroundDrawable(d);
		  					
		  					printWriter.print("Stop");
				  			printWriter.flush();
		  					
		  				}
		  				return true;
		  			}
		  		});
		          
	          
	          } catch (UnknownHostException e) {
	  	        e.printStackTrace();
	          } catch (IOException e) {    
	  	        e.printStackTrace();
	          }
          //}
        //});
        
        /*
        alert.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
          public void onClick(DialogInterface dialog, int whichButton) {
            // Canceled.
        	  dialog.cancel();
        	  finish();
          }
        });
    
        alert.show();
        */
	}
	
	public void setProgressBarColor(ProgressBar progressBar, int newColor){ 
	    LayerDrawable ld = (LayerDrawable) progressBar.getProgressDrawable();
	    ClipDrawable d1 = (ClipDrawable) ld.findDrawableByLayerId(R.id.progressshape);
	    d1.setColorFilter(newColor, PorterDuff.Mode.SRC_IN);

	}
	
}



