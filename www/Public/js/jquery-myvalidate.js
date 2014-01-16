$.validator.addMethod("cnRequired", $.validator.methods.required, "<span class=\"label label-danger\">此项为必填项</span>");


$.validator.addClassRules("required-cn", { cnRequired: true });



$.validator.addMethod("cnNumber", $.validator.methods.number, "<span class=\"label label-danger\">此项必须为数字</span>");


$.validator.addClassRules("number-cn", { cnNumber: true });

$(".required-cn").each(function() {
	var myprev = $(this).parent().prev();
	myprev.append(" <a style=\"color: red\">*</a>");
});
