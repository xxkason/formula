package com.mcnetdog.bluetoothcar;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends Activity {
    static final int ENABLE_BLUETOOTH_REQUEST = 1;
    Button btnUp, btnDown, btnLeft, btnRight, btnScan;

    private BluetoothAdapter myBluetooth = null;
    private BluetoothSocket btSocket = null;
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
