package com.example.bipedalpid;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.Intent;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

import app.akexorcist.bluetotohspp.library.BluetoothSPP;
import app.akexorcist.bluetotohspp.library.BluetoothState;
import app.akexorcist.bluetotohspp.library.DeviceList;

public class MainActivity extends AppCompatActivity implements SensorEventListener{

    private double kp = 0.0;
    private double ki = 0.0;
    private double kd = 0.0;

    private BluetoothSPP bt;

    private Button btnConnect; //연결시도
    private Button btnSend; //데이터 전송
    private Button btnPIDReset;
    private EditText etSerial;
    private TextView tvSensorOrientation;
    private TextView tvP;
    private TextView tvI;
    private TextView tvD;
    private Switch toggleSwitch;

    private float[] mR = new float[9];
    private float[] mOrientation = new float[3];

    private SensorManager mSensorManager;
    private Sensor mAccelerometer;
    private Sensor mMagnetometer;
    private float[] mLastAccelerometer = new float[3];
    private float[] mLastMagnetometer = new float[3];
    private boolean mLastAccelerometerSet = false;
    private boolean mLastMagnetometerSet = false;

    private boolean readBt = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ActionBar actionBar = getSupportActionBar();
        actionBar.hide();

        btnConnect = findViewById(R.id.btnConnect); //연결시도
        btnSend = findViewById(R.id.btnSend); //데이터 전송
        btnPIDReset = findViewById(R.id.btnPIDReset);
        etSerial = findViewById(R.id.etSerial);
        tvSensorOrientation = findViewById(R.id.tvSensorOrientation);
        tvP = findViewById(R.id.tvP);
        tvI = findViewById(R.id.tvI);
        tvD = findViewById(R.id.tvD);
        toggleSwitch = findViewById(R.id.toggleSwitch);

        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        mMagnetometer = mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);

        bt = new BluetoothSPP(this); //Initializing

        if (!bt.isBluetoothAvailable()) { //블루투스 사용 불가
            Toast.makeText(getApplicationContext()
                    , "Bluetooth is not available"
                    , Toast.LENGTH_SHORT).show();
            finish();
        }

        bt.setOnDataReceivedListener(new BluetoothSPP.OnDataReceivedListener() { //데이터 수신
            public void onDataReceived(byte[] data, String message) {
                Toast.makeText(MainActivity.this, message, Toast.LENGTH_SHORT).show();
            }
        });

        bt.setBluetoothConnectionListener(new BluetoothSPP.BluetoothConnectionListener() { //연결됐을 때
            public void onDeviceConnected(String name, String address) {
                Toast.makeText(getApplicationContext()
                        , "Connected to " + name + "\n" + address
                        , Toast.LENGTH_SHORT).show();
            }

            public void onDeviceDisconnected() { //연결해제
                Toast.makeText(getApplicationContext()
                        , "Connection lost", Toast.LENGTH_SHORT).show();
            }

            public void onDeviceConnectionFailed() { //연결실패
                Toast.makeText(getApplicationContext()
                        , "Unable to connect", Toast.LENGTH_SHORT).show();
            }
        });

        btnConnect.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                if (bt.getServiceState() == BluetoothState.STATE_CONNECTED) {
                    bt.disconnect();
                } else {
                    Intent intent = new Intent(getApplicationContext(), DeviceList.class);
                    startActivityForResult(intent, BluetoothState.REQUEST_CONNECT_DEVICE);
                }
            }
        });

        btnPIDReset.setOnClickListener(new View.OnClickListener(){
            public void onClick(View v) {
                bt.send("P0.0"+'\n', true);
                bt.send("I0.0"+'\n', true);
                bt.send("D0.0"+'\n', true);
                tvP.setText("KP0.0");
                tvI.setText("KI0.0");
                tvD.setText("KD0.0");
            }
        });

        toggleSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked)
                {
                    readBt = true;
                }
                else
                {
                    readBt = false;
                }
            }
        });
    }

    @Override
    protected void onResume(){
        super.onResume();
        mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL);
        mSensorManager.registerListener( this, mMagnetometer, SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    protected void onPause(){
        super.onPause();
        mSensorManager.unregisterListener( this, mAccelerometer);
        mSensorManager.unregisterListener( this, mMagnetometer);
    }

    @Override
    public void onSensorChanged(SensorEvent event){
        if (readBt) {
            if (event.sensor == mAccelerometer) {
                System.arraycopy(event.values, 0, mLastAccelerometer, 0, event.values.length);
                mLastAccelerometerSet = true;
            } else if (event.sensor == mMagnetometer) {
                System.arraycopy(event.values, 0, mLastMagnetometer, 0, event.values.length);
                mLastMagnetometerSet = true;
            }
            if (mLastAccelerometerSet && mLastMagnetometerSet) {
                mSensorManager.getRotationMatrix(mR, null, mLastAccelerometer, mLastMagnetometer);
                //float pitch = (int)(Math.toDegrees(mSensorManager.getOrientation(mR, mOrientation)[1]) + 360) % 360;
                float[] newR = {mR[0], mR[2], -mR[1], mR[3], mR[5], -mR[4], mR[6], mR[8], -mR[7]};
                double pitch = -(int) (Math.toDegrees(mSensorManager.getOrientation(newR, mOrientation)[1]) * 100) / 100.0;
                tvSensorOrientation.setText("PITCH: " + Double.toString(pitch) + " degrees");
                bt.send("R" + pitch, true);
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy){}

    public void onDestroy() {
        super.onDestroy();
        bt.stopService(); //블루투스 중지
    }

    public void onStart() {
        super.onStart();
        if (!bt.isBluetoothEnabled()) { //
            Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(intent, BluetoothState.REQUEST_ENABLE_BT);
        } else {
            if (!bt.isServiceAvailable()) {
                bt.setupService();
                bt.startService(BluetoothState.DEVICE_OTHER); //DEVICE_ANDROID는 안드로이드 기기 끼리
                setup();
            }
        }
    }

    public void setup() {
        btnSend.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                String msg = etSerial.getText().toString();
                bt.send(msg+'\n', true);

                if (msg.charAt(0) == 'P'){
                    tvP.setText("K"+msg);
                }
                else if (msg.charAt(0) == 'I'){
                    tvI.setText("K"+msg);
                }
                else if (msg.charAt(0) == 'D'){
                    tvD.setText("K"+msg);
                }

                etSerial.setText("");
            }
        });
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == BluetoothState.REQUEST_CONNECT_DEVICE) {
            if (resultCode == Activity.RESULT_OK)
                bt.connect(data);
        } else if (requestCode == BluetoothState.REQUEST_ENABLE_BT) {
            if (resultCode == Activity.RESULT_OK) {
                bt.setupService();
                bt.startService(BluetoothState.DEVICE_OTHER);
                setup();
            } else {
                Toast.makeText(getApplicationContext()
                        , "Bluetooth was not enabled."
                        , Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }
}