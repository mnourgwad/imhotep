package com.spells.batterycharge;

import android.content.Intent;
import android.graphics.Color;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;

public class OverallBattery extends AppCompatActivity implements View.OnTouchListener{

    private int noOfBatteries;
    private float[] readings;
    private float maxReading;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        noOfBatteries = 3;
        readings = new float[]{253, 276, 264};
        maxReading = 350;

        OverallBatteryView overallBatteryView = new OverallBatteryView(this, noOfBatteries, readings, maxReading);
        overallBatteryView.setBackgroundColor(Color.WHITE);
        setContentView(overallBatteryView);
        overallBatteryView.setOnTouchListener(this);

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
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        int action = event.getAction();

        if (action == MotionEvent.ACTION_UP) {
            Intent intent = new Intent(this, SeparateBatteries.class);
            intent.putExtra("NoOfBatteries", noOfBatteries);
            intent.putExtra("Readings", readings);
            intent.putExtra("MaxReading", maxReading);
            startActivity(intent);
        }

        return true;
    }
}
