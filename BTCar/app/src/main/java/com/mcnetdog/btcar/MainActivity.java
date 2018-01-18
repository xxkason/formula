package com.mcnetdog.btcar;

import android.bluetooth.BluetoothSocket;
import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.Toast;
import android.content.Intent;
import android.bluetooth.BluetoothAdapter;
import java.util.Timer;
import android.bluetooth.BluetoothDevice;

import java.io.IOException;
import java.util.TimerTask;
import java.util.Set;

public class MainActivity extends AppCompatActivity implements OnTouchListener {
    static final int ENABLE_BLUETOOTH_REQUEST = 1;
    Button btnUp, btnDown, btnLeft, btnRight, btnScan;

    private BluetoothAdapter myBluetooth = null;
    private BluetoothSocket btSocket = null;

    Timer timer = new Timer(true);
    TimerTask turnLeftTask = new TimerTask() {
        @Override
        public void run() {
            sendCommand("l");
        }
    };
    TimerTask turnRightTask = new TimerTask() {
        @Override
        public void run() {
            sendCommand("r");
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        myBluetooth = BluetoothAdapter.getDefaultAdapter();
        Set<BluetoothDevice> pairedDevices = null;

        if (myBluetooth == null)
        {
            //Show a message. that the device has no bluetooth adapter
            msg("Bluetooth Device Not Available");
            //finish apk
            finish();
        }
        else
        {
            if (!myBluetooth.isEnabled())
            {
                //Ask to the user turn the bluetooth on
                Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(turnBTon,ENABLE_BLUETOOTH_REQUEST);
            }
            else
            {
                pairedDevices = myBluetooth.getBondedDevices();
                if (pairedDevices.size() > 0)
                {
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
                }
            }
        }

        btnScan = this.findViewById(R.id.btnConnect);
        btnUp = this.findViewById(R.id.btnUp);
        btnDown = this.findViewById(R.id.btnDown);
        btnLeft = this.findViewById(R.id.btnLeft);
        btnRight = this.findViewById(R.id.btnRight);

        btnUp.setOnTouchListener(this);
        btnDown.setOnTouchListener(this);
        btnLeft.setOnTouchListener(this);
        btnRight.setOnTouchListener(this);

/*        btnUp.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
                {
                    sendCommand("f");
                }
                else if (motionEvent.getAction() == MotionEvent.ACTION_UP)
                {
                    sendCommand("s");
                }
                return true;
            }
        });

        btnDown.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
                {
                    sendCommand("b");
                }
                else if (motionEvent.getAction() == MotionEvent.ACTION_UP)
                {
                    sendCommand("s");
                }
                return true;
            }
        });

        btnLeft.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
                {
                    timer.schedule(turnLeftTask, 100, 400);

                }
                else if (motionEvent.getAction() == MotionEvent.ACTION_UP)
                {
                    timer.cancel();
                    sendCommand("c");
                }
                return true;
            }
        });

        btnRight.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
                {
                    timer.schedule(turnRightTask, 100, 400);
                }
                else if (motionEvent.getAction() == MotionEvent.ACTION_UP)
                {
                    timer.cancel();
                    sendCommand("c");
                }
                return true;
            }
        });*/
    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {
        if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
        {
            switch (view.getId()) {
                case R.id.btnUp:
                    sendCommand("f");
                    break;
                case R.id.btnDown:
                    sendCommand("b");
                    break;
                case R.id.btnLeft:
                    timer.schedule(turnLeftTask, 100, 400);
                    break;
                case R.id.btnRight:
                    timer.schedule(turnRightTask, 100, 400);
                    break;
            }
        }
        else if (motionEvent.getAction() == MotionEvent.ACTION_UP)
        {
            switch (view.getId()) {
                case R.id.btnUp:
                case R.id.btnDown:
                    sendCommand("s");
                    break;
                case R.id.btnLeft:
                case R.id.btnRight:
                    //timer.cancel();
                    sendCommand("c");
                    break;
            }
        }
        return true;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data){
        if (requestCode == ENABLE_BLUETOOTH_REQUEST)
        {
            if (resultCode == RESULT_CANCELED)
                finish();
        }
    }

    private void sendCommand (String cmd){
        if (btSocket != null && btSocket.isConnected())
        {
            try
            {
                btSocket.getOutputStream().write(cmd.toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
        else
        {
            msg("Not connected to any device");
        }
    }

    private void msg(final String s)
    {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
            }
        });
    }
}
