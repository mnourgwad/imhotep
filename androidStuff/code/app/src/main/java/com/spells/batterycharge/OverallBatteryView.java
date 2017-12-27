package com.spells.batterycharge;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

import java.text.DecimalFormat;

public class OverallBatteryView extends View {

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

    public OverallBatteryView(Context context, int noOfBatteries, float[] readings, float maxReading) {
        super(context);
        init();
        this.noOfBatteries = noOfBatteries;
        this.readings      = readings;
        this.maxReading    = maxReading;
        decimalFormat = new DecimalFormat("#.#");
    }

    public OverallBatteryView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public OverallBatteryView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        float width = (float) getWidth();
        float height = (float) getHeight();

        float start_x = (width / 2) - 100;
        float start_y = (height / 2) - 200;

        float start_x_cap = (width / 2) - 50;
        float start_y_cap = (height / 2) - 230;

        float overallCharge = 0;
        for (float reading : readings) {
            overallCharge += reading;
        }

        float overallChargePercentage = (overallCharge / (maxReading * noOfBatteries)) * 100;

        canvas.drawRoundRect(start_x, start_y, start_x + 200, start_y + 400, 4, 4, paintBattery);
        canvas.drawRoundRect(start_x_cap, start_y_cap, start_x_cap + 100, start_y_cap + 30, 4, 4, paintCap);
        canvas.drawRoundRect(start_x, start_y + 120, start_x + 200, start_y + 400, 4, 4, paintCharge);
        canvas.drawText(decimalFormat.format(overallChargePercentage) + "%", start_x + 50, start_y + 200, paintText);
        canvas.drawText(decimalFormat.format(overallCharge) + "/" + decimalFormat.format(maxReading * noOfBatteries), start_x + 20, start_y + 440, paintText);
    }

    @Override
    public boolean performClick() {
        return super.performClick();
    }
}
