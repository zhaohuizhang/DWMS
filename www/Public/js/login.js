//看不清，换一个验证码

function change_code() {
    var myDate = new Date();
    var myTime = myDate.toLocaleString();
    $("#code").attr("src", verifyURL + '/' + myTime);
    return false;
}
