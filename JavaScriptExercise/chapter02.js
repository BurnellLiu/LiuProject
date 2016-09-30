/**
 * Created by Jie on 9/29/2016.
 */


window.onload = function (){
    var ans = prompt("Enter a number", "");
    try {
        if (!ans || isNaN(ans) || ans < 0)
            throw new Error("Not a number");

        alert("The square root of " + ans + "is" + Math.sqrt(ans));
    }
    catch (errMsg){
        alert(errMsg.message);

    }
};

function writeMessage(){
    document.getElementById("helloMessage").innerHTML = "Hello, world!";
    /*
    if (confirm("Are you sure want to do that?")){
        alert("You said yes");
    }
    else {
        alert("You said no");
    }
     */

}

function initAll(){
    document.getElementById("redirect").onclick = function(){
        alert("Leave our website")
        window.location = "http://www.qq.com";

        // 返回false表示停止对用户单机的处理， 这样就不会加载href指向的页面
        return false;
    };
}
