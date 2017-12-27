package com.spells.batterycharge;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

import java.text.DecimalFormat;

public class SeparateBatteriesView extends View {

    private Paint paintBattery;
    private Paint paintCap;
    private Paint paintCharge;
    private Paint paintText;

    private int noOfBatteries;
    private float[] readings;
    private float maxReading;

    private DecimalFormat decimalFormat;

    private void init() {
        paintBattery = new Paint();
        paintBattery.setColor(0x99212121);
        paintBattery.setStyle(Paint.Style.STROKE);
        paintBattery.setStrokeWidth(4);

        paintCap = new Paint();
        paintCap.setColor(0x99212121);
        paintCap.setStyle(Paint.Style.FILL);
        paintCap.setStrokeWidth(4);
        paintCap.setTextSize(40);

        paintCharge = new Paint();
        paintCharge.setColor(0x9900BFA5);
        paintCharge.setStyle(Paint.Style.FILL);
        paintCharge.setStrokeWidth(4);

        paintText = new Paint();
        paintText.setColor(0x99212121);
        paintText.setStyle(Paint.Style.FILL);
        paintText.setTextSize(40);
    }

    public SeparateBatteriesView(Context context, int noOfBatteries, float[] readings, float maxReading) {
        super(context);
        init();
        this.noOfBatteries = noOfBatteries;
        this.readings      = readings;
        this.maxReading    = maxReading;
        decimalFormat = new DecimalFormat("#.#");
    }

    public SeparateBatteriesView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public SeparateBatteriesView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        float width = (float) getWidth();
        float height = (float) getHeight();

        if (noOfBatteries <= 3) {
            float lengthBetweenBatteries = ((width - (noOfBatteries * 200)) / (noOfBatteries + 1));
            float start_x = lengthBetweenBatteries;
            float start_y = (height / 2) - 200;

            for (int i = 0; i < noOfBatteries; i++) {
                float chargePercentage = (readings[i] / maxReading) * 100;
                float start_x_charge   = start_x;
                float start_y_charge   = start_y + (400 - (chargePercentage * 4));

                float start_x_cap = start_x + 50;
                float start_y_cap = start_y - 30;

                canvas.drawRoundRect(start_x, start_y, start_x + 200, start_y + 400, 4, 4, paintBattery);
                canvas.drawRoundRect(start_x_charge, start_y_charge, start_x + 200, start_y + 400, 4, 4, paintCharge);
                canvas.drawRoundRect(start_x_cap, start_y_cap, start_x_cap + 100, start_y_cap + 30, 4, 4, paintCap);
                canvas.drawText(decimalFormat.format(chargePercentage) + "%", start_x + 50, start_y + 200, paintText);
                canvas.drawText(decimalFormat.format(readings[i]) + "/" + decimalFormat.format(maxReading), start_x + 30, start_y + 440, paintText);
                start_x += (lengthBetweenBatteries + 200);
            }
        }
    }
}
