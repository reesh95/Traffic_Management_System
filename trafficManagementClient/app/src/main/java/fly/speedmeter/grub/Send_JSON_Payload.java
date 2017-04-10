package fly.speedmeter.grub;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;

import java.io.IOException;

import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;

public class Send_JSON_Payload extends Service {
    Data data;
    private Location loc;
    public double latitude;
    public double longitude;
    public float dir;
    public Send_JSON_Payload() {
        data = MainActivity.getData();
        OkHttpClient client = new OkHttpClient();
        //locationManager = (LocationManager)  this.getSystemService(Context.LOCATION_SERVICE);
        loc= MainActivity.getloc();
        //locationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
        if (loc != null) {
            latitude = loc.getLatitude();
            longitude = loc.getLongitude();
            dir = loc.getBearing();
            MediaType mediaType = MediaType.parse("application/json");
            RequestBody body = RequestBody.create(mediaType, "{\"Average_speed\":" + data.getAverageSpeed().subSequence(0, data.getAverageSpeed().length() - 4) + ", \"Distance\": " + data.getDistance().subSequence(0, data.getDistance().length() - 1) + ", \"Location\": \"" + String.valueOf(latitude).subSequence(0,7) + ", " + String.valueOf(longitude).subSequence(0,7) + "\", \"Direction\":" + String.valueOf(dir) + "}\n");
            //RequestBody body = RequestBody.create(mediaType, "{\"Average_speed\":10, \"Distance\": 123, \"Location\": \"12.95, 77.19\",\"Direction\":0}\n");
            Request request = new Request.Builder()
                    .url("http://52.74.108.222/Thingworx/Things/Phoneapp_thing/Services/UpdateValues?postParameter=Payload")
                    .post(body)
                    .addHeader("content-type", "application/json")
                    .addHeader("appkey", "0c9070d7-9a13-4677-bd8a-f9297b8d296c")
                    .addHeader("accept", "application/html")
                    .addHeader("cache-control", "no-cache")
                    .addHeader("postman-token", "12972401-1670-7d7c-afdd-01f30a23e770")
                    .build();
            try {
                Response response = client.newCall(request).execute();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }


    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }
}
