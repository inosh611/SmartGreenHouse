package com.example.myapplication;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    FirebaseDatabase  database;
    DatabaseReference databaseReference;

    TextView textView;
    TextView humidity;
    TextView soilmoistuer;

    TextView light;
    TextView fan;
    TextView motor;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView = findViewById(R.id.textView);
        humidity =findViewById(R.id.textViewH);
        soilmoistuer = findViewById(R.id.SoilId);
        light = findViewById(R.id.light1);
        fan = findViewById(R.id.fan);
        motor = findViewById(R.id.motor);



        database =FirebaseDatabase.getInstance();
        databaseReference = database.getReference("GreenHouse").child("Values");


        databaseReference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if(snapshot.exists()) {
                    float temp = snapshot.child("Temperature").getValue(float.class);
                    textView.setText(String.valueOf(temp)+" °C");

                    if(temp>=30){
                        fan.setText("ON");
                    }else{
                        fan.setText("OFF");
                    }

                } else {
                    Log.d("DB Test", "No data");
                }

                if(snapshot.exists()) {
                    float hum = snapshot.child("Humidity").getValue(float.class);
                    humidity.setText(String.valueOf(hum)+" %");

                } else {
                    Log.d("DB Test", "No data");
                }
                if(snapshot.exists()) {
                    float soil = snapshot.child("SoilMoisture").getValue(float.class);
                    soilmoistuer.setText(String.valueOf(soil)+" %");

                    if(soil<50){
                        motor.setText("ON");
                    }else{
                        motor.setText("OFF");
                    }

                } else {
                    Log.d("DB Test", "No data");
                }

                if(snapshot.exists()) {
                    float ldr = snapshot.child("LDRValue").getValue(float.class);
                    if(ldr==0){
                        light.setText("OFF");
                    }else{
                        light.setText("ON");
                    }

                } else {
                    Log.d("DB Test", "No data");
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });
    }
}