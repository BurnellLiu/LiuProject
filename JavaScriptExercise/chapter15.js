/**
 * Created by Jie on 2016/11/6.
 */

// 设置页面被加载完成后的处理函数
$(document).ready(function() {
    // 选择id为welcome的标签， 再该标签最后追加指定内容
    $("#welcome").append("Welcome to jQuery");

    // 选择所有的a标签， 设置a标签的点击处理函数
    $("a").click(function(event){
        // 选择id为colorMe的标签， 设置该标签的class属性
        // this表示当前点击的标签
        $("#colorMe").attr("class", $(this).attr("id"));
        // 通知浏览器点击行为已经被处理过， 阻止默认行为的发生
        event.preventDefault();
    });

    // 选择所有的a标签， 设置hover函数， 改变颜色
    $("a").hover(function(){
        $(this).css({
            "color": $(this).attr("id")
        });
    });

    $("a").mouseout(function(){
       $(this).css({
           "color": "white"
       });
    });

    var stateList = "Alabama*Alaska*Arizona*Arkansas*California*Colorado*Connecticut*Delaware*Florida*Georgia*Hawaii*Idaho*Illinois*Indiana*Iowa*Kansas*Kentucky*Louisiana*Maine*Maryland*MassachusettsMichigan*Minnesota*Mississippi*Missouri*Montana*Nebraska*Nevada*New Hampshire*New Jersey*New MexicoNew York*North Carolina*North Dakota*Ohio*Oklahoma*Oregon*Pennsylvania*Rhode Island*South CarolinaSouth Dakota*Tennessee*Texas*Utah*Vermont*Virginia*Washington*West Virginia*Wisconsin*Wyoming*";
    $("#searchField").autocomplete({
        source: stateList.split("*")
    });

});






