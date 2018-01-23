package com.mcnetdog.bluetoothcar;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
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
    ImageButton btnUp, btnDown, btnLeft, btnRight;

    private BluetoothAdapter myBluetooth = null;
    private BluetoothSocket btSocket = null;
    private static final UUID MY_UUID_SECURE = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    private final BroadcastReceiver bluetoothStatusReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (action.equals(BluetoothAdapter.ACTION_CONNECTION_STATE_CHANGED))
            {
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_CONNECTION_STATE, BluetoothAdapter.ERROR);
                switch (state)
                {
                    case BluetoothAdapter.STATE_CONNECTED:
                        getActionBar().setSubtitle("Connected");
                        invalidateOptionsMenu();
                        break;
                    case BluetoothAdapter.STATE_DISCONNECTED:
                        getActionBar().setSubtitle("Not Connected");
                        invalidateOptionsMenu();
                        break;
                    case BluetoothAdapter.STATE_CONNECTING:
                        getActionBar().setSubtitle("Connecting...");
                        break;
                    case BluetoothAdapter.STATE_DISCONNECTING:
                        getActionBar().setSubtitle("Disconnecting...");
                        break;
                }
            }
        }
    };

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
        IntentFilter btStateFilter = new IntentFilter(BluetoothAdapter.ACTION_CONNECTION_STATE_CHANGED);
        registerReceiver(bluetoothStatusReceiver, btStateFilter);
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

        btnUp.setOnTouchListener(this);
        btnDown.setOnTouchListener(this);
        btnLeft.setOnTouchListener(this);
        btnRight.setOnTouchListener(this);
    }

    @Override
    protected void onDestroy(){
        super.onDestroy();
        unregisterReceiver(bluetoothStatusReceiver);
    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {
        if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
        {
            switch (view.getId()) {
                case R.id.upBtn:
                    sendCommand("f");
                    break;
                case R.id.downBtn:
                    sendCommand("b");
                    break;
                case R.id.leftBtn:
                    timer.schedule(turnLeftTask, 100, 400);
                    break;
                case R.id.rightBtn:
                    timer.schedule(turnRightTask, 100, 400);
                    break;
            }
        }
        else if (motionEvent.getAction() == MotionEvent.ACTION_UP)
        {
            switch (view.getId()) {
                case R.id.upBtn:
                case R.id.downBtn:
                    sendCommand("s");
                    break;
                case R.id.leftBtn:
                case R.id.rightBtn:
                    timer.cancel();
                    sendCommand("c");
                    break;
            }
        }
        return true;
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
                return true;
            case R.id.action_disconnect:
                try
                {
                    if (btSocket != null)
                        btSocket.close();
                }
                catch (IOException e)
                {
                    msg("Close bluetooth connection failed.");
                }
            default:
                // If we got here, the user's action was not recognized.
                // Invoke the superclass to handle it.
                return super.onOptionsItemSelected(item);
        }
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
