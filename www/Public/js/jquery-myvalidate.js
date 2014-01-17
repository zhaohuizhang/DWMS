$.validator.addMethod("cnRequired", $.validator.methods.required, "<span class=\"label label-danger\">此项为必填项</span>");


$.validator.addClassRules("required-cn", { cnRequired: true });



$.validator.addMethod("cnNumber", $.validator.methods.number, "<span class=\"label label-danger\">此项必须为数字</span>");


$.validator.addClassRules("number-cn", { cnNumber: true });

$(".required-cn").each(function() {
	var myprev = $(this).parent().prev();
	myprev.append(" <a style=\"color: red\">*</a>");
});


$.validator.addMethod("cnPwdEqual", function (value, element) {
			// bind to the blur event of the target in order to revalidate whenever the target field is updated
			// TODO find a way to bind the event just once, avoiding the unbind-rebind overhead
			var param="#password"
			var target = $(param);
			/* if ( this.settings.onfocusout ) {
				target.unbind(".validate-pwdEqual").bind("blur.validate-pwdEqual", function() {
					$(element).valid();
				});
			}*/
			return value === target.val();
		}, "<span class=\"label label-danger\">两次密码必须一致</span>");

$.validator.addClassRules("pwdEqual", { cnPwdEqual: true });
