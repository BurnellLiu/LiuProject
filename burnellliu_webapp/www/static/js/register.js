

/**
 * 验证邮箱的是否有效
 * @param {String} email 邮箱字符串
 * @returns {boolean} true(合法邮箱)， false(非法邮箱)
 */
function validateEmail(email){
    var re = /^[a-z0-9\.\-\_]+\@[a-z0-9\-\_]+(\.[a-z0-9\-\_]+){1,4}$/;
    return re.test(email.toLowerCase());
}

/**
 * 显示错误消息
 * @param {String} msg 错误消息
 */
function showErrorMessage(msg){

    // 找到显示错误消息的标签
    var $alert = $('#vm').find(".uk-alert-danger");
    if ($alert.length === 0){
        return;
    }

    if (!msg){
        return;
    }

    $alert.text(msg);
    $alert.removeClass('uk-hidden');
}

function requestDone(data){
    console.log(data);
}

function postAccount(data){
    var opt = {
        type: 'POST',
        url: '/api/users',
        dataType: 'json',
        data: JSON.stringify(data || {}),
        contentType: 'application/json'
    };

    var jqxhr = $.ajax(opt);
    jqxhr.done(requestDone);
}

/**
 * 账号提交处理函数
 * @param event
 */
function accountSubmit(event){
    // 通知浏览器提交已被处理， 阻止默认行为的发生
    event.preventDefault();

    // 检查账号信息是否合法
    if (!this.name.trim()){
        showErrorMessage('请输入名字');
        return;
    }
    if (!validateEmail(this.email.trim().toLowerCase())) {
        showErrorMessage('请输入正确的Email地址');
        return;
    }
    if (this.password1.length < 6){
        showErrorMessage('密码长度至少为6个字符');
        return;
    }
    if (this.password1 !== this.password2){
        showErrorMessage('两次输入的密码不一致');
        return;
    }

    var email = this.email.trim().toLowerCase();
    var account = {
        name: this.name.trim(),
        email: email,
        password: CryptoJS.SHA1(email + ':' + this.password1).toString()
    };

    // 将账号信息POST出去
    postAccount(account);
}


/**
 * 初始化页面
 */
function initPage(){

    // 创建模型
    var model = {
        el: '#vm',
        data: {
            name: '',
            email: '',
            password1: '',
            password2: ''
        },
        methods: {
            submit: accountSubmit
        }
    };
    var vm = new Vue(model);

    // 显示视图
    $('#vm').show();
}


$(document).ready(initPage);

/*
$(function () {
    var vm = new Vue({
        el: '#vm',
        data: {
            name: '',
            email: '',
            password1: '',
            password2: ''
        },
        methods: {
            submit: function (event) {
                event.preventDefault();
                var $form = $('#vm');
                if (! this.name.trim()) {
                    //return $form.showFormError('请输入名字');
                    return showErrorMessage('请输入名字');
                }
                if (! validateEmail(this.email.trim().toLowerCase())) {
                    return $form.showFormError('请输入正确的Email地址');
                }
                if (this.password1.length < 6) {
                    return $form.showFormError('密码长度至少为6个字符');
                }
                if (this.password1 !== this.password2) {
                    return $form.showFormError('两次输入的密码不一致');
                }
                var email = this.email.trim().toLowerCase();
                $form.postJSON('/api/users', {
                    name: this.name.trim(),
                    email: email,
                    password: CryptoJS.SHA1(email + ':' + this.password1).toString()
                }, function (err, r) {
                    if (err) {
                        return $form.showFormError(err);
                    }
                    return location.assign('/');
                });
            }
        }
    });
    $('#vm').show();
});*/