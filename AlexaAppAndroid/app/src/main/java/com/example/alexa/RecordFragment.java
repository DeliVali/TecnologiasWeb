package com.example.alexa;

import android.Manifest;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.media.MediaRecorder;
import android.net.Uri;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.fragment.app.Fragment;

import android.os.CountDownTimer;
import android.os.Environment;
import android.os.Handler;
import android.speech.RecognizerIntent;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Locale;

import static android.app.Activity.RESULT_OK;


public class RecordFragment extends Fragment implements View.OnClickListener {

private ImageButton recordBtn;
private boolean isRecording=false;
private String recordPermission=Manifest.permission.RECORD_AUDIO;
private int PERMISSION_CODE=21;
private MediaRecorder mediaRecorder;
private String recordFile;
private CountDownTimer mCountdowntimer;
private TextView anuncio;
private String command;
    protected static final int RESULT_SPEECH = 1;
    private int countdownPeriod=5000;
    public RecordFragment() {
        // Required empty public constructor
    }



    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_record, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState){
        super.onViewCreated(view,savedInstanceState);
        anuncio=view.findViewById(R.id.record_filename);
      recordBtn=view.findViewById(R.id.record_btn);

      recordBtn.setOnClickListener(this);
    }

    //Switch para el boton del microfono
    @Override
    public void onClick(View v) {
        //Programa para crear archivo de audio
        //Mejor solucion mas abajo
        /*if (isRecording){
            //Imagen del boton cuando este apagado
            //Stop Recording
            stopRecording();
            isRecording = false;
            recordBtn.setImageDrawable(getResources().getDrawable(R.drawable.mic));
            anuncio.setText("Press the mic button to start talking");
            connectMQTT();

        }else{
            //imagen cuando empiece a grabar

            if (checkPermissions()) {
                // Start Recording

                startRecording();

                anuncio.setText("Listening...");
                recordBtn.setImageDrawable(getResources().getDrawable(R.drawable.recording));


                    mCountdowntimer = new CountDownTimer(countdownPeriod, 5000) {//countdown Period =5000

                        public void onTick(long millisUntilFinished) {
                            isRecording = true;
                        }

                        public void onFinish() {
                            //Si cancela el audio el usuario antes de que termine el timer no pone el is recording como false ,
                            //Por lo tanto regresa a la funcion on click y hace la validacion y la detiene
                            if (isRecording==false) {
                                mCountdowntimer.cancel();
                            }else {
                                //Si espera a que termine el audio entonces sigue su flujo normal y el audio se detiene despues del tiempo
                                //Si hace la validacion on click el is return sera false ,por lo tanto podra iniciar otra vez la grabación
                               mediaRecorder.stop();
                                isRecording = false;
                                recordBtn.setImageDrawable(getResources().getDrawable(R.drawable.mic));
                                anuncio.setText("Press the mic button to start talking");
                                //En este apartado se mandaria el archivo mediante la conexion bluetooh
                                connectMQTT();
                            }

                        }

                    }.start();

                }
            }

        */


        Intent intent = new Intent(
                RecognizerIntent.ACTION_RECOGNIZE_SPEECH);

        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, "en-US");

        try {
            startActivityForResult(intent, RESULT_SPEECH);

        } catch (ActivityNotFoundException a) {
            Toast t = Toast.makeText(getActivity(),
                    "Opps! Your device doesn't support Speech to Text",
                    Toast.LENGTH_SHORT);
            t.show();
        }




    }




//Metodo para parar de grabar
    /*private void stopRecording() {
        mediaRecorder.stop();
        mediaRecorder.release();
        mediaRecorder=null;

    }*/


//Metodo para iniciar la grabacion
    //Graba el audio en un arhivo
    /*private void startRecording() {
        String recordPath= getActivity().getExternalFilesDir("/").getAbsolutePath();
        recordFile="Command.3gp";
        mediaRecorder= new MediaRecorder();
        mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
        mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
        mediaRecorder.setOutputFile(recordPath + "/" + recordFile);
        mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);

        try {
            mediaRecorder.prepare();
        } catch (IOException e) {
            e.printStackTrace();
        }
        mediaRecorder.start();




    }
*/

    private boolean checkPermissions() {
        if (ActivityCompat.checkSelfPermission(getContext(), recordPermission )== PackageManager.PERMISSION_GRANTED){
        return true;

        }else {
            ActivityCompat.requestPermissions(getActivity(),new String[]{recordPermission},PERMISSION_CODE);
            return false;
        }

    }





//Metodo para conectar al servidor  mqtt
    public void connectMQTT(){

        String clientId = MqttClient.generateClientId();
        MqttAndroidClient client =
                //Declara el contexto , la ip del servidor mqtt y el nombre del usuario
                //Tienes que abrir los puertos de tu windows
                //1)firewall.cpl
                new MqttAndroidClient(getActivity(), "tcp://192.168.1.2:1883",
                        clientId);

        try {
            MqttConnectOptions options = new MqttConnectOptions();
            options.setKeepAliveInterval(60);
            Log.d("", "MqttConnectOptions : "+options.toString());
            IMqttToken token = client.connect(options);

            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    // We are connected
                    Log.d("TAG", "onSuccess");
                    Toast.makeText(getActivity(), "Se Conecto", Toast.LENGTH_SHORT).show();

                    //Envio de mensajes al servidor mqtt
                    try {
                        //Declara el output

                        client.subscribe("/", 0);

                        //Manda el mensaje
                        client.publish("Commands", new MqttMessage(command.getBytes()));

                        Toast.makeText(getActivity(), "Comando Enviado", Toast.LENGTH_SHORT).show();
                    } catch (MqttException ex) {

                    }



                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    // Something went wrong e.g. connection timeout or firewall problems
                    Log.d("TAG", "onFailure");
                    Toast.makeText(getActivity(), "No Se Conecto", Toast.LENGTH_SHORT).show();

                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }



    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        switch (requestCode) {
            case RESULT_SPEECH: {
                if (resultCode == RESULT_OK && null != data) {

                    ArrayList<String> text = data
                            .getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);

                    this.command=text.get(0);
                }
                break;
            }

        }
        connectMQTT();
    }


    public static void printLog(Context context){
        String filename = context.getExternalFilesDir(null).getPath() + File.separator + "my_app.log";
        String command = "logcat -f "+ filename + " -v time *:V";

        Log.d("TAG", "command: " + command);

        try{
            Runtime.getRuntime().exec(command);
        }
        catch(IOException e){
            e.printStackTrace();
        }
    }




}