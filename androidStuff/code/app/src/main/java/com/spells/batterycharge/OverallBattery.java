package com.spells.batterycharge;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.os.AsyncTask;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;

import com.spells.batterycharge.Enums.ConnectionStatus;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;

public class OverallBattery extends AppCompatActivity {


    private OverallBatteryView overallBatteryView;

    private int noOfBatteries = 0;
    private float[] readings = null;
    private float maxReading = 0;
    private float conversionFactor;

    private EditText arduinoIpEdtTxt;
    private EditText arduinoPortEdtTxt;

    private String arduinoIp;
    private int arduinoPort;

    private Socket socket = null;
    // private BufferedReader inputStream = null;
    private OutputStream outputStream = null;

    private ConnectionStatus connectionStatus;

    private boolean cancelChangeArduinoAddress = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayOptions(actionBar.getDisplayOptions() | ActionBar.DISPLAY_SHOW_CUSTOM);

            ImageView imageView = new ImageView(actionBar.getThemedContext());
            imageView.setScaleType(ImageView.ScaleType.CENTER);
            imageView.setImageResource(R.drawable.imhotep_logo_small);

            ActionBar.LayoutParams layoutParams = new ActionBar.LayoutParams(
                    ActionBar.LayoutParams.WRAP_CONTENT,
                    ActionBar.LayoutParams.WRAP_CONTENT,
                    Gravity.END
            );
            layoutParams.rightMargin = 40;

            imageView.setLayoutParams(layoutParams);
            actionBar.setCustomView(imageView);
        }

        connectToArduino();
    }

    private void connectToArduino() {
        AlertDialog.Builder dialogBuilder = new AlertDialog.Builder(this);
        LayoutInflater inflater = this.getLayoutInflater();
        View dialogView = inflater.inflate(R.layout.arduino_address_dialog, null);
        dialogBuilder.setTitle("Enter Arduino address:");
        dialogBuilder.setView(dialogView);
        arduinoIpEdtTxt = dialogView.findViewById(R.id.arduino_ip);
        arduinoPortEdtTxt = dialogView.findViewById(R.id.arduino_port);
        dialogBuilder.setCancelable(false);
        dialogBuilder.setPositiveButton("Submit", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                arduinoIp = arduinoIpEdtTxt.getText().toString();
                arduinoPort = Integer.parseInt(arduinoPortEdtTxt.getText().toString());

                wifiConnect();
            }
        });

        if (cancelChangeArduinoAddress) {
            dialogBuilder.setCancelable(true);
            dialogBuilder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {

                }
            });
        }

        AlertDialog alertDialog = dialogBuilder.create();
        alertDialog.show();
    }

    private void wifiConnect() {
        new WifiTask(arduinoIp, arduinoPort, this).execute();
    }

    private void constructUI() {
        if (noOfBatteries != 0 || readings != null || maxReading != 0) {
            overallBatteryView = new OverallBatteryView(this, noOfBatteries, readings, maxReading, connectionStatus);
            overallBatteryView.setBackgroundColor(Color.WHITE);

            setContentView(overallBatteryView);
            overallBatteryView.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    int action = event.getAction();

                    if (action == MotionEvent.ACTION_DOWN) {
                        Intent intent = new Intent(OverallBattery.this, SeparateBatteries.class);
                        intent.putExtra("NoOfBatteries", noOfBatteries);
                        intent.putExtra("Readings", readings);
                        intent.putExtra("MaxReading", maxReading);
                        startActivity(intent);
                    }

                    return true;
                }
            });
        } else {
            Log.e("CONSTRUCT_UI", "Can't construct UI");
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.config_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int itemClicked = item.getItemId();

        switch (itemClicked) {
            case R.id.no_of_batteries:
                setConfigData("Batteries");
                break;
            case R.id.max_charge:
                setConfigData("MaxReading");
                break;
            case R.id.conversion_factor:
                setConfigData("ConversionFactor");
                break;
            case R.id.reconnect:
                wifiConnect();
                break;
            case R.id.connect_information:
                cancelChangeArduinoAddress = true;
                connectToArduino();
                break;
            default:
                break;
        }

        return true;
    }

    private void setConfigData(String dataToBeSet) {
        final EditText dataNewValue;

        AlertDialog.Builder builder;
        AlertDialog alertDialog;

        builder = new AlertDialog.Builder(this);

        LayoutInflater inflater = this.getLayoutInflater();
        View dialogView = inflater.inflate(R.layout.set_config_data_dialog, null);
        builder.setView(dialogView);
        dataNewValue = dialogView.findViewById(R.id.data_new_value);

        switch (dataToBeSet) {
            case "Batteries":
                dataNewValue.setHint("New No. Of batteries");
                builder.setTitle("No. Of Batteries");
                builder.setPositiveButton("Submit", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        noOfBatteries = Integer.parseInt(dataNewValue.getText().toString());
                        constructUI();
                    }
                }).setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                alertDialog = builder.create();
                alertDialog.show();
                break;
            case "MaxReading":
                dataNewValue.setHint("New Max Charge");
                builder.setTitle("Max Charge");
                builder.setPositiveButton("Submit", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        maxReading = Integer.parseInt(dataNewValue.getText().toString());
                        constructUI();
                    }
                }).setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                alertDialog = builder.create();
                alertDialog.show();
                break;
            case "ConversionFactor":
                dataNewValue.setHint("New Conversion factor");
                builder.setTitle("Conversion Factor");
                builder.setPositiveButton("Submit", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        conversionFactor = Integer.parseInt(dataNewValue.getText().toString());
                        constructUI();
                    }
                }).setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                alertDialog = builder.create();
                alertDialog.show();
                break;
            default:
                break;
        }
    }

    /*
     * Wifi connection AsyncTask
     * adapted from https://github.com/hmartiro/android-arduino-wifi
     */
    private class WifiTask extends AsyncTask<Void, Void, ConnectionStatus> {

        private String arduinoIp;
        private int arduinoPort;

        private Context context;

        private int timeout = 10000;

        private AlertDialog.Builder builder;
        private AlertDialog alertDialog;

        WifiTask(String arduinoIp, int arduinoPort, Context context) {
            this.arduinoIp = arduinoIp;
            this.arduinoPort = arduinoPort;
            this.context = context;
        }

        @Override
        protected void onPreExecute() {
            builder = new AlertDialog.Builder(context);
            builder.setMessage("Connecting...");
            builder.setCancelable(false);
            alertDialog = builder.create();
            alertDialog.show();
        }

        @Override
        protected ConnectionStatus doInBackground(Void... voids) {

            socket = new Socket();
            try {
                socket.connect(new InetSocketAddress(arduinoIp, arduinoPort), timeout);
                // inputStream  = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                outputStream = socket.getOutputStream();

                if (socket.isConnected()) {
                    Log.e("CONNECT", "CONNECTED");
                    connectionStatus = ConnectionStatus.CONNECTED;
                } else {
                    Log.e("CONNECT", "Socket did not connect!");
                    connectionStatus = ConnectionStatus.FAILURE;
                }
            } catch (IOException e) {
                // e.printStackTrace();
                Log.e("CONNECT", "Socket did not connect!");
                connectionStatus = ConnectionStatus.FAILURE;
            }
            return connectionStatus;
        }

        @Override
        protected void onPostExecute(ConnectionStatus connectionStatus) {
            alertDialog.cancel();

            if (connectionStatus == ConnectionStatus.CONNECTED) {
                Log.e("QUERY", "Starting");
                new ConfigDataQuery(context, outputStream, socket, "ConfigData").execute();
                // new ConfigDataQuery(this, outputStream, socket, "Batteries").execute();
                // new ConfigDataQuery(this, outputStream, socket, "MaxReading").execute();
                // new ConfigDataQuery(this, outputStream, socket, "Readings").execute();
            }
        }
    }

    private class ConfigDataQuery extends AsyncTask<Void, Void, Void> {

        private Context context;
        private OutputStream outputStream;
        private String query;
        private Socket socket;

        private AlertDialog.Builder builder;
        private AlertDialog alertDialog;

        ConfigDataQuery(Context context, OutputStream outputStream, Socket socket, String query) {
            this.context = context;
            this.outputStream = outputStream;
            this.socket = socket;
            this.query = query;
        }

        @Override
        protected void onPreExecute() {
            builder = new AlertDialog.Builder(context);
            builder.setMessage("Querying...");
            builder.setCancelable(false);
            alertDialog = builder.create();
            alertDialog.show();
        }

        @Override
        protected Void doInBackground(Void... voids) {
            try {
                Log.e("QUERY", query);
                outputStream.write(query.getBytes());
                outputStream.write('\n');
                String data = new BufferedReader(new InputStreamReader(socket.getInputStream())).readLine();
                /* switch (query) {
                    case "Batteries":
                        noOfBatteries = Integer.parseInt(data);
                        break;
                    case "MaxReading":
                        maxReading = Integer.parseInt(data);
                        break;
                    case "Readings":
                        String readingsArray = data;
                        break;
                } */
                Log.e(query, data);

                String[] configData = data.split("-");
                noOfBatteries = Integer.parseInt(configData[0]);
                maxReading = Integer.parseInt(configData[1]);
                readings = new float[noOfBatteries];

                String[] readingsData = configData[2].split(",");
                for (int i = 0; i < noOfBatteries; i++) {
                    readings[i] = Float.parseFloat(readingsData[i]);
                }

            } catch (IOException e) {
                e.printStackTrace();
                Log.e("QUERY", "Can't query " + query);
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            alertDialog.cancel();
            constructUI();
        }
    }
}
