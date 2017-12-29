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
        float width  = (float) getWidth();
        float height = (float) getHeight();

        float batteryWidth;
        float batteryHeight;

        float batteryCapHeight;
        float batteryTextHeight;

        if (height < 1500) {
            batteryWidth  = 125;
            batteryHeight = 250;

            batteryCapHeight   = 20;
            batteryTextHeight = 30;

            paintText.setTextSize(25);
        } else {
            batteryWidth  = 200;
            batteryHeight = 400;

            batteryCapHeight   = 30;
            batteryTextHeight = 40;

            paintText.setTextSize(40);
        }

        if (noOfBatteries <= 3) {
            float start_y = height / 2;
            drawBatteries(canvas, width, noOfBatteries, start_y, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, readings);
        } else if (noOfBatteries > 3 && noOfBatteries <= 6) {
            float start_y = height / 2;
            if (noOfBatteries == 4) {
                float topPart = start_y / 2;
                float[] topBatteriesReadings = {readings[0], readings[1]};
                drawBatteries(canvas, width, 2, topPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, topBatteriesReadings);

                float[] downBatteriesReadings = {readings[2], readings[3]};
                float downPart = (float) (start_y * 1.5);
                drawBatteries(canvas, width, 2, downPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, downBatteriesReadings);
            } else if (noOfBatteries == 5) {
                float topPart = start_y / 2;
                float[] topBatteriesReadings = {readings[0], readings[1]};
                drawBatteries(canvas, width, 2, topPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, topBatteriesReadings);

                float[] downBatteriesReadings = {readings[2], readings[3], readings[4]};
                float downPart = (float) (start_y * 1.5);
                drawBatteries(canvas, width, 3, downPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, downBatteriesReadings);
            } else if (noOfBatteries == 6) {
                float topPart = start_y / 2;
                float[] topBatteriesReadings = {readings[0], readings[1], readings[2]};
                drawBatteries(canvas, width, 3, topPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, topBatteriesReadings);

                float[] downBatteriesReadings = {readings[3], readings[4], readings[5]};
                float downPart = (float) (start_y * 1.5);
                drawBatteries(canvas, width, 3, downPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, downBatteriesReadings);
            }
        } else if (noOfBatteries > 6 && noOfBatteries <= 8) {
            float start_y = height / 3;
            if (noOfBatteries == 7) {
                float firstPart = start_y / 2;
                float[] firstBatteriesReadings = {readings[0], readings[1]};
                drawBatteries(canvas, width, 2, firstPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, firstBatteriesReadings);

                float secondPart = (float) (start_y * 1.5);
                float[] secondBatteriesReadings = {readings[2], readings[3]};
                drawBatteries(canvas, width, 2, secondPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, secondBatteriesReadings);

                float thirdPart = (float) (start_y * 2.5);
                float[] thirdBatteriesReadings = {readings[4], readings[5], readings[6]};
                drawBatteries(canvas, width, 3, thirdPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, thirdBatteriesReadings);
            } else if (noOfBatteries == 8) {
                float firstPart = start_y / 2;
                float[] firstBatteriesReadings = {readings[0], readings[1], readings[2]};
                drawBatteries(canvas, width, 3, firstPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, firstBatteriesReadings);

                float secondPart = (float) (start_y * 1.5);
                float[] secondBatteriesReadings = {readings[3], readings[4]};
                drawBatteries(canvas, width, 2, secondPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, secondBatteriesReadings);

                float thirdPart = (float) (start_y * 2.5);
                float[] thirdBatteriesReadings = {readings[5], readings[6], readings[7]};
                drawBatteries(canvas, width, 3, thirdPart, batteryWidth, batteryHeight, batteryCapHeight, batteryTextHeight, thirdBatteriesReadings);
            }
        }
    }

    private void drawBatteries(Canvas canvas,
                               float width,
                               int noOfBatteriesToDraw,
                               float start_y,
                               float batteryWidth,
                               float batteryHeight,
                               float batteryCapHeight,
                               float batteryTextHeight,
                               float[] currentBatteriesReadings) {
        float lengthBetweenBatteries = ((width - (noOfBatteriesToDraw * batteryWidth)) / (noOfBatteriesToDraw + 1));
        float start_x = lengthBetweenBatteries;
        start_y -= (batteryHeight / 2);

        for (int i = 0; i < noOfBatteriesToDraw; i++) {
            float chargePercentage = (currentBatteriesReadings[i] / maxReading) * 100;
            float start_x_charge   = start_x;
            float start_y_charge   = start_y + (batteryHeight - (chargePercentage * (batteryHeight / 100)));

            float start_x_cap = start_x + (batteryWidth / 4);
            float start_y_cap = start_y - batteryCapHeight;

            canvas.drawRoundRect(start_x, start_y, start_x + batteryWidth, start_y + batteryHeight, 4, 4, paintBattery);
            canvas.drawRoundRect(start_x_charge, start_y_charge, start_x + batteryWidth, start_y + batteryHeight, 4, 4, paintCharge);
            canvas.drawRoundRect(start_x_cap, start_y_cap, start_x_cap + (batteryWidth / 2), start_y_cap + batteryCapHeight, 4, 4, paintCap);
            canvas.drawText(decimalFormat.format(chargePercentage) + "%", start_x + (batteryWidth / 4), start_y + (batteryHeight / 2), paintText);
            canvas.drawText(decimalFormat.format(currentBatteriesReadings[i]) + "/" + decimalFormat.format(maxReading), start_x + (batteryWidth / 6), start_y + (batteryHeight + batteryTextHeight), paintText);
            start_x += (lengthBetweenBatteries + batteryWidth);
        }
    }
}
