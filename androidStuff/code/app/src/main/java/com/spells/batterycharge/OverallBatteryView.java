package com.spells.batterycharge;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;

import com.spells.batterycharge.Enums.ConnectionStatus;

import java.text.DecimalFormat;

public class OverallBatteryView extends View {

    private Paint paintBattery;
    private Paint paintCap;
    private Paint paintCharge;
    private Paint paintText;

    private int noOfBatteries;
    private float[] readings;
    private float maxReading;
    private ConnectionStatus connectionStatus;

    private DecimalFormat decimalFormat;

    private Bitmap footerLogos;

    private Bitmap wifiConnected;
    private Bitmap wifiDisconnected;
    private Bitmap wifiFailure;

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

    public OverallBatteryView(Context context, int noOfBatteries, float[] readings, float maxReading, ConnectionStatus connectionStatus) {
        super(context);
        init();
        this.noOfBatteries    = noOfBatteries;
        this.readings         = readings;
        this.maxReading       = maxReading;
        this.connectionStatus = connectionStatus;
        decimalFormat = new DecimalFormat("#.#");

        footerLogos = BitmapFactory.decodeResource(context.getResources(), R.drawable.footer_logos);

        wifiConnected    = BitmapFactory.decodeResource(context.getResources(), R.drawable.wifi_green);
        wifiDisconnected = BitmapFactory.decodeResource(context.getResources(), R.drawable.wifi_red);
        wifiFailure      = BitmapFactory.decodeResource(context.getResources(), R.drawable.wifi_gray);
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

        if (connectionStatus == ConnectionStatus.CONNECTED) {
            canvas.drawBitmap(wifiConnected, null, new Rect(35, (int) height - 150, 80, (int) height - 110), null);
        } else if (connectionStatus == ConnectionStatus.DISCONNECTED) {
            canvas.drawBitmap(wifiDisconnected, null, new Rect(35, (int) height - 150, 80, (int) height - 110), null);
        } else {
            canvas.drawBitmap(wifiFailure, null, new Rect(35, (int) height - 150, 80, (int) height - 110), null);
        }

        canvas.drawBitmap(footerLogos, null, new Rect(20, (int) height - 80, (int) width - 40, (int) height), null);
    }

    @Override
    public boolean performClick() {
        return super.performClick();
    }
}
