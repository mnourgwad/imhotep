package com.spells.batterycharge;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.view.Gravity;
import android.widget.ImageView;

public class SeparateBatteries extends AppCompatActivity {

    private int noOfBatteries;
    private float[] readings;
    private float maxReading;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();
        noOfBatteries = intent.getIntExtra("NoOfBatteries", 0);
        readings      = intent.getFloatArrayExtra("Readings");
        maxReading    = intent.getFloatExtra("MaxReading", 0);

        SeparateBatteriesView separateBatteriesView = new SeparateBatteriesView(this, noOfBatteries, readings, maxReading);
        separateBatteriesView.setBackgroundColor(Color.WHITE);
        setContentView(separateBatteriesView);

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

            actionBar.setDisplayHomeAsUpEnabled(true);
        }
    }
}
