package com.mcnetdog.bluetoothcar;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Vibrator;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;
import java.util.UUID;

public class MainActivity extends Activity implements View.OnTouchListener {
    static final int ENABLE_BLUETOOTH_REQUEST = 1;
    static final int SEEK_BAR_STEP = 1;
    static final int SEEK_BAR_MIN = 0;
    static final int SEEK_BAR_MAX = 255;
    ImageButton btnUp, btnDown, btnLeft, btnRight;
    TextView speedText;
    SeekBar speedBar;
    Timer timer = null;
    Vibrator vibe = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);

    private BluetoothAdapter myBluetooth = null;
    private BluetoothSocket btSocket = null;
    private static final UUID MY_UUID_SECURE = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getActionBar().setSubtitle(R.string.connect_state);
        myBluetooth = BluetoothAdapter.getDefaultAdapter();
        if (myBluetooth == null){
            msg("Bluetooth is not supported by this device");
            finish();
        }
        else if (!myBluetooth.isEnabled()){
            Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnBTon,ENABLE_BLUETOOTH_REQUEST);
        }

        btnUp = this.findViewById(R.id.upBtn);
        btnDown = this.findViewById(R.id.downBtn);
        btnLeft = this.findViewById(R.id.leftBtn);
        btnRight = this.findViewById(R.id.rightBtn);
        speedText = this.findViewById(R.id.speedValue);
        speedBar = this.findViewById(R.id.speedBar);
        speedBar.setMax((SEEK_BAR_MAX - SEEK_BAR_MIN) / SEEK_BAR_STEP);

        btnUp.setOnTouchListener(this);
        btnDown.setOnTouchListener(this);
        btnLeft.setOnTouchListener(this);
        btnRight.setOnTouchListener(this);
        speedBar.setOnSeekBarChangeListener(new speedChangeListener());
    }

    private class speedChangeListener implements SeekBar.OnSeekBarChangeListener {
        public void onProgressChanged(SeekBar seekBar, int progress,
                                      boolean fromUser) {
            int speed = SEEK_BAR_MIN + (progress * SEEK_BAR_STEP);
            speedText.setText(speed);
        }

        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        public void onStopTrackingTouch(SeekBar seekBar) {
            sendCommand(String.format("i:%d", seekBar.getProgress()));
        }
    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {
        if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
        {
            vibe.vibrate(100);
            switch (view.getId()) {
                case R.id.upBtn:
                    btnUp.setBackgroundColor(Color.CYAN);
                    sendCommand("s:f");
                    break;
                case R.id.downBtn:
                    btnDown.setBackgroundColor(Color.CYAN);
                    sendCommand("s:b");
                    break;
                case R.id.leftBtn:
                    btnLeft.setBackgroundColor(Color.CYAN);
                    startTimer("s:l");
                    break;
                case R.id.rightBtn:
                    btnRight.setBackgroundColor(Color.CYAN);
                    startTimer("s:r");
                    break;
            }
        }
        else if (motionEvent.getAction() == MotionEvent.ACTION_UP)
        {
            switch (view.getId()) {
                case R.id.upBtn:
                    btnUp.setBackgroundColor(Color.WHITE);
                    sendCommand("s:s");
                    break;
                case R.id.downBtn:
                    btnDown.setBackgroundColor(Color.WHITE);
                    sendCommand("s:s");
                    break;
                case R.id.leftBtn:
                    btnLeft.setBackgroundColor(Color.WHITE);
                    stopTimer();
                    sendCommand("s:c");
                    break;
                case R.id.rightBtn:
                    btnRight.setBackgroundColor(Color.WHITE);
                    stopTimer();
                    sendCommand("s:c");
                    break;
            }
        }
        return true;
    }

    private void startTimer(final String cmd)
    {
        timer = new Timer(true);
        TimerTask timeTask = new TimerTask() {
            @Override
            public void run() {
                sendCommand(cmd);
            }
        };
        timer.schedule(timeTask, 100, 400);
    }

    private void stopTimer()
    {
        if (timer != null)
            timer.cancel();
    }

    private void sendCommand (String cmd){
        if (btSocket != null && btSocket.isConnected())
        {
            try
            {
                btSocket.getOutputStream().write(cmd.getBytes());
            }
            catch (IOException e)
            {
                msg("Send out command to connected device failed");
            }
        }
        else
        {
            msg("Not connected to any device");
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu){
        getMenuInflater().inflate( R.menu.ic_action_buttons, menu );
        menu.findItem(R.id.action_bluetooth).setVisible(btSocket == null || !btSocket.isConnected());
        menu.findItem(R.id.action_disconnect).setVisible(btSocket != null && btSocket.isConnected());
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        switch (item.getItemId()){
            case R.id.action_bluetooth:
                showPairedList();
                break;
            case R.id.action_disconnect:
                try
                {
                    if (btSocket != null)
                        btSocket.close();
                    updateSubtitle();
                    invalidateOptionsMenu();
                }
                catch (IOException e)
                {
                    msg("Close bluetooth connection failed.");
                }
                break;
            default:
                // If we got here, the user's action was not recognized.
                // Invoke the superclass to handle it.
                return super.onOptionsItemSelected(item);
        }
        return true;
    }

    public void showPairedList() {
        Set<BluetoothDevice> pairedDevices = myBluetooth.getBondedDevices();
        ArrayList list = new ArrayList();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice bt : pairedDevices){
                list.add(bt.getName() + "\n" + bt.getAddress());
            }
        }
        else
        {
            msg("No paired devices");
            return;
        }
        ListView deviceList = new ListView(this);
        deviceList.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
        final ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1, list);
        deviceList.setAdapter(adapter);
        //Setup Alert Dialog
        AlertDialog.Builder dialogBuilder = new AlertDialog.Builder(this);
        dialogBuilder.setTitle(R.string.dialog_title)
                .setView(deviceList);
        final AlertDialog pairedDeviceDialog = dialogBuilder.create();
        deviceList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                // Get the device MAC address, the last 17 chars in the View
                String info = ((TextView) view).getText().toString();
                String address = info.substring(info.length() - 17);
                pairedDeviceDialog.dismiss();
                try
                {
                    if (btSocket == null || !btSocket.isConnected())
                    {
                        BluetoothDevice device2Connect = myBluetooth.getRemoteDevice(address);
                        btSocket = device2Connect.createRfcommSocketToServiceRecord(MY_UUID_SECURE);
                        myBluetooth.cancelDiscovery();
                        btSocket.connect();
                    }
                }
                catch (IOException e)
                {
                    msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                }
                updateSubtitle();
                invalidateOptionsMenu();
            }
        });
        pairedDeviceDialog.show();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data){
        if (requestCode == ENABLE_BLUETOOTH_REQUEST)
        {
            if (resultCode == RESULT_CANCELED)
                finish();
        }
    }

    private void updateSubtitle()
    {
        if (btSocket != null && btSocket.isConnected())
        {
            getActionBar().setSubtitle("Connected");
        }
        else
        {
            getActionBar().setSubtitle("Not Connected");
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
