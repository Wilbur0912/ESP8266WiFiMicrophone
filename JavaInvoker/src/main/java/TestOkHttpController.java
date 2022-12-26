import com.alibaba.fastjson2.JSON;
import org.apache.catalina.User;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.HashMap;
import java.util.Map;

@RestController
public class TestOkHttpController {


    /*@GetMapping("/test/form")
    public String sendForm() {
        String url = "http://localhost:8010/test/form";
        Map<String, String> params = new HashMap<>();
        params.put("name", "libai");
        params.put("age", "1");
        / OkHttpUtil.post(url, params);
    }*/


   /* @GetMapping("/test/user")
    public String sendUser() {
        String url = "http://localhost:8010/test/user";
        User user= new User();
        user.setName("平安");
        user.setAge(12);
        return OkHttpUtil.post(url, null, JSON.toJSONString(user));
    }*/
   @GetMapping("/test/img")
   public String sendImg() {
       String url = "http://localhost:8010/test/img";
       return OkHttpUtil.postImg(url, null, "{}");
   }
}