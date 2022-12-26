import okhttp3.*;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.net.URL;

public class Client {
    private static byte[] getInputStream(String urlStr) {
        byte[] data = null;
        try {
            FileOutputStream fos = new FileOutputStream("C:\\Users\\weber\\OneDrive\\桌面\\testFile\\fata2.wav");
            //錄音
            URL url = new URL(urlStr);
            BufferedInputStream bis = new BufferedInputStream(url.openStream());
            System.out.println("錄音中...");
            data = bis.readAllBytes();
            System.out.println(data.length);
            fos.write(data, 0, data.length);
            //存進SQL
            bis.close();
            fos.close();
            return data;
        } catch (Exception e) {

        }
        return data;
    }
    public static void main(String[] args) throws Exception{
        getInputStream("http://192.168.4.1/send");
        File file = new File("C:\\Users\\weber\\OneDrive\\桌面\\testFile\\fata2.wav");
        System.out.println(file.length() + "2次大小");
        OkHttpClient client = new OkHttpClient().newBuilder()
                .build();
        MediaType mediaType = MediaType.parse("multipart/form-data");
        RequestBody body = new MultipartBody.Builder().setType(MultipartBody.FORM)
                .addFormDataPart("file", "C:\\Users\\weber\\OneDrive\\桌面\\testFile\\fata2.wav",
                        RequestBody.create(MediaType.parse("application/octet-stream"),
                                new File("C:\\Users\\weber\\OneDrive\\桌面\\testFile\\fata2.wav")))
                .addFormDataPart("id", "test")
                .build();
        Request request = new Request.Builder()
                .url("http://123.205.91.234:8080/db/upload")
                        .method("POST", body)
                        .build();
        Response response = client.newCall(request).execute();
        System.out.println(response);
    }
}
