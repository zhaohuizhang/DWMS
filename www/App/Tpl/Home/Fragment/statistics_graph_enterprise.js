option = {
    tooltip : {
        trigger: 'item',
        formatter: "{a} <br/>{b} : {c} ({d}%)"
    },
    legend: {
        orient : 'vertical',
        x : 'left',
        data:['直达','营销广告','搜索引擎','邮件营销','联盟广告','视频广告','百度','谷歌','必应','其他']
    },
    toolbox: {
        show : true,
        feature : {
            mark : true,
            dataView : {readOnly: false},
            restore : true,
            saveAsImage : true
        }
    },
    calculable : false,
    series : [
        {
            name:'访问来源',
            type:'pie',
            selectedMode: 'single',
            radius : [0, 70],
            itemStyle : {
                normal : {
                    label : {
                        position : 'inner'
                    },
                    labelLine : {
                        show : false
                    }
                }
            },
            data:[
                {value:335, name:'直达'},
                {value:679, name:'营销广告'},
                {value:1548, name:'搜索引擎', selected:true}
            ]
        },
        {
            name:'访问来源',
            type:'pie',
            radius : [100, 140],
            data:[
                {value:335, name:'直达'},
                {value:310, name:'邮件营销'},
                {value:234, name:'联盟广告'},
                {value:135, name:'视频广告'},
                {value:1048, name:'百度'},
                {value:251, name:'谷歌'},
                {value:147, name:'必应'},
                {value:102, name:'其他'}
            ]
        }
    ]
};
var ecConfig = require('echarts/config');
myChart.on(ecConfig.EVENT.PIE_SELECTED, function(param){
    var selected = param.selected;
    var serie;
    var str = '当前选择： ';
    for (var idx in selected) {
        serie = option.series[idx];
        for (var i = 0, l = serie.data.length; i < l; i++) {
            if (selected[idx][i]) {
                str += '【系列' + idx + '】' + serie.name + ' : ' + 
                       '【数据' + i + '】' + serie.data[i].name + ' ';
            }
        }
    }
    document.getElementById('statistics_graph_enterprise').innerHTML = str;
})
