import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

public class Record {
    public static final String ADD_URL = "http://123.205.91.234:8080/db/upload";
    private static byte[] getInputStream(String urlStr) {
        byte[] data = null;
        try {
            FileOutputStream fos = new FileOutputStream("C:\\Users\\tonyp\\Desktop\\wavFile\\fata.wav");
            //錄音
            URL url = new URL(urlStr);
            BufferedInputStream bis = new BufferedInputStream(url.openStream());
            System.out.println("錄音中...");
            data = bis.readAllBytes();
            fos.write(data, 0, data.length);
            //存進SQL
            bis.close();
            fos.close();
            return data;
        } catch (Exception e) {

        }
        return data;
    }
    public static void appadd(byte[] data) {

        System.out.println("data長度" + data.length);
        File file = new File("C:\\Users\\tonyp\\Desktop\\wavFile\\fata.wav");
        System.out.println(file.length() + "appadd方法");
        StringBuffer stringBuffer = new StringBuffer();
        try {
            Map<String, Object> map = new HashMap<>();
            map.put("file", file);
            map.put("id", "123123");
            map.put("token", "123123");
            /*
            //建立連線
            URL url = new URL(ADD_URL);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setDoOutput(true);
            connection.setDoInput(true);
            connection.setRequestMethod("POST");
            connection.setUseCaches(false);
            connection.setInstanceFollowRedirects(true);

            connection.setRequestProperty("Content-Type","application/x-www-form-urlencoded");
            connection.setRequestProperty("Content-Type","multipart/form-data;");

            connection.connect();

            //POST請求
            DataOutputStream out = new DataOutputStream(connection.getOutputStream());


            //out.write(obj.toString().getBytes("UTF-8"));
            out.flush();
            out.close();

            //讀取響應
            BufferedReader reader = new BufferedReader(new InputStreamReader(
                    connection.getInputStream()));
            String lines;
            StringBuffer sb = new StringBuffer("");
            while ((lines = reader.readLine()) != null) {
                lines = new String(lines.getBytes(), "utf-8");
                sb.append(lines);
            }
            System.out.println(sb);
            reader.close();
            // 斷開連線
            connection.disconnect();
        } catch (MalformedURLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (UnsupportedEncodingException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
           */
        } catch (Exception e) {
            System.out.println(e);
        }
    }
    public static void main(String[] args) {
        appadd(getInputStream("http://192.168.4.1/send"));
    }
}
