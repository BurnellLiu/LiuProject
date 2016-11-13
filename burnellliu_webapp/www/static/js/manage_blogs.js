

/*
function initVM(data) {
    var vm = new Vue({
        el: '#vm',
        data: {
            blogs: data.blogs,
            page: data.page
        },
        methods: {
            edit_blog: function (blog) {
                location.assign('/manage/blogs/edit?id=' + blog.id);
            },
            delete_blog: function (blog) {
                if (confirm('确认要删除“' + blog.name + '”？删除后不可恢复！')) {
                    postJSON('/api/blogs/' + blog.id + '/delete', function (err, r) {
                        if (err) {
                            return alert(err.message || err.error || err);
                        }
                        refresh();
                    });
                }
            }
        }
    });
    $('#vm').show();
}

$(function() {
    getJSON('/api/blogs', {
        page: {{ page_index }}
    }, function (err, results) {
        if (err) {
            return fatal(err);
        }
        $('#loading').hide();
        initVM(results);
    });
});
    */

if (!Number.prototype.toDateTime) {
    var replaces = {
        'yyyy': function(dt) {
            return dt.getFullYear().toString();
        },
        'yy': function(dt) {
            return (dt.getFullYear() % 100).toString();
        },
        'MM': function(dt) {
            var m = dt.getMonth() + 1;
            return m < 10 ? '0' + m : m.toString();
        },
        'M': function(dt) {
            var m = dt.getMonth() + 1;
            return m.toString();
        },
        'dd': function(dt) {
            var d = dt.getDate();
            return d < 10 ? '0' + d : d.toString();
        },
        'd': function(dt) {
            var d = dt.getDate();
            return d.toString();
        },
        'hh': function(dt) {
            var h = dt.getHours();
            return h < 10 ? '0' + h : h.toString();
        },
        'h': function(dt) {
            var h = dt.getHours();
            return h.toString();
        },
        'mm': function(dt) {
            var m = dt.getMinutes();
            return m < 10 ? '0' + m : m.toString();
        },
        'm': function(dt) {
            var m = dt.getMinutes();
            return m.toString();
        },
        'ss': function(dt) {
            var s = dt.getSeconds();
            return s < 10 ? '0' + s : s.toString();
        },
        's': function(dt) {
            var s = dt.getSeconds();
            return s.toString();
        },
        'a': function(dt) {
            var h = dt.getHours();
            return h < 12 ? 'AM' : 'PM';
        }
    };
    var token = /([a-zA-Z]+)/;
    Number.prototype.toDateTime = function(format) {
        var fmt = format || 'yyyy-MM-dd hh:mm:ss'
        var dt = new Date(this * 1000);
        var arr = fmt.split(token);
        for (var i=0; i<arr.length; i++) {
            var s = arr[i];
            if (s && s in replaces) {
                arr[i] = replaces[s](dt);
            }
        }
        return arr.join('');
    };
}

/**
 * 显示错误消息
 * @param {String} msg 错误消息, 如果设置为null, 则隐藏错误消息标签
 */
function showErrorMessage(msg){

    // 找到显示错误消息的标签
    var $alert = $('#error');
    if ($alert.length === 0){
        return;
    }

    if (!msg){
        $alert.hide();
        return;
    }

    $alert.text(msg);
    $alert.show();
}

/**
 * 设置数据是否处于加载状态
 * @param {Boolean} isLoading true(加载状态), false(非加载状态)
 */
function showDataLoading(isLoading){
    if (isLoading){
        $('#loading').show();
    }
    else {
        $('#loading').hide();
    }

}

/**
 * 删除博客请求结束处理函数
 */
function deleteBlogRequestDone(data){
    // 如果有错则显示错误消息
    if (data.error){
        showErrorMessage(data.message);
        return;
    }
    // 如果成功删除博客, 则刷新页面
    var url = location.pathname;
    location.assign(url);
}

/**
 * 发送删除博客请求
 * @param {String} blogId 博客Id
 */
function postDeleteBlogRequest(blogId){
    var opt = {
        type: 'POST',
        url: '/api/blogs/' + blogId + '/delete',
        dataType: 'json',
        data: JSON.stringify({}),
        contentType: 'application/json'
    };
    // 发送请求
    var jqxhr = $.ajax(opt);
    // 设置请求完成和请求失败的处理函数
    jqxhr.done(deleteBlogRequestDone);
    jqxhr.fail(requestFail);
}

/**
 * 编辑博客处理函数
 * @param {Object} e 标签对象
 */
function editIconClicked(e){
    var id = $(e).attr("blog-id");
    location.assign('/manage/blogs/edit?id=' + id);
}

/**
 * 删除博客处理函数
 * @param {Object} e 标签对象
 */
function trashIConClicked(e){
    var id = $(e).attr('blog-id');
    var name = $(e).attr('blog-name');
    if (confirm('确认要删除"' + name + '"?删除后不可恢复!')){
        postDeleteBlogRequest(id);
    }
}

/**
 * 显示博客数据
 * @param {Object} data 博客数据
 */
function showBlogsData(data){
    // 创建博客表
    var $table = $('#blogs-table tbody');
    var blogs = data.blogs;
    for (var i in blogs){
        $table.append(
            '<tr >' +
            '<td>' +
            '<a target="_blank" href="/blog/' + blogs[i].id + '">' + blogs[i].name + '</a>' +
            '</td>' +
            '<td>' +
            '<a target="_blank">' + blogs[i].user_name + '</a>'+
            '</td>' +
            '<td>' +
            '<span>'+ blogs[i].created_at.toDateTime() + '</span>'+
            '</td>' +
            '<td>' +
            '<a href="#0" onclick="editIconClicked(this)" blog-id="' + blogs[i].id + '">' +
            '<i class="uk-icon-edit"></i> </a>' +
            '<a href="#0" onclick="trashIConClicked(this)" blog-id="' + blogs[i].id + '" blog-name="' + blogs[i].name + '">' +
            '<i class="uk-icon-trash-o"></i></a>' +
            '</td>' +
            '</tr>');
    }

    // 创建分页列
    var $ul = $('ul.uk-pagination');
    if (data.page.has_previous){
        var prePageIndex = parseInt(data.page.page_index) - 1;
        var previousLi =
            '<li>' +
            '<a href="/manage/blogs?page=' + prePageIndex + '">' +
            '<i class="uk-icon-angle-double-left"></i>' +
            '</a>' +
            '</li>'
    }
    else {
        var previousLi =
            '<li class="uk-disabled">' +
            '<span><i class="uk-icon-angle-double-left"></i></span>' +
            '</li>';
    }
    $ul.append(previousLi);

    $ul.append('<li class="uk-active"><span>' + data.page.page_index + '</span></li>');

    if (data.page.has_next){
        var nextPageIndex = parseInt(data.page.page_index) + 1;
        var nextLi =
            '<li>' +
            '<a href="/manage/blogs?page='+ nextPageIndex + '">' +
            '<i class="uk-icon-angle-double-right"></i>' +
            '</a>' +
            '</li>';
    }
    else {
        var nextLi =
            '<li class="uk-disabled">' +
            '<span><i class="uk-icon-angle-double-right"></i></span>' +
            '</li>';
    }
    $ul.append(nextLi);

}

/**
 * 获取博客请求结束处理函数
 * @param {Object} data 返回的数据
 */
function getBlogsRequestDone(data){
    showDataLoading(false);

    // 如果有错则显示错误消息
    if (data.error){
        showErrorMessage(data.message);
        return;
    }

    showBlogsData(data);
}

/**
 * 请求错误处理函数
 * @param xhr
 * @param status
 */
function requestFail(xhr, status){
    showDataLoading(false);
    showErrorMessage('网络出了问题 (HTTP '+ xhr.status+')');
}

/**
 * 发送获取博客信息请求
 * @param {String} pageIndex 页面索引
 */
function postGetBlogsRequest(pageIndex){
    var opt = {
        type: 'GET',
        url: '/api/blogs?page=' + pageIndex,
        dataType: 'json',
    };
    // 发送请求
    var jqxhr = $.ajax(opt);
    // 设置请求完成和请求失败的处理函数
    jqxhr.done(getBlogsRequestDone);
    jqxhr.fail(requestFail);
}

/**
 * 初始化页面
 */
function initPage(){
    showErrorMessage(null);
    showDataLoading(true);

    var pageIndex = location.search.replace('?page=', '');
    postGetBlogsRequest(pageIndex);
}

$(document).ready(initPage)