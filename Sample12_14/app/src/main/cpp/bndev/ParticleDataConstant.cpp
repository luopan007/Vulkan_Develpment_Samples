const float X_RANGE[]={//粒子发射的x 范围
                1.0f,
                1.0f,
                1.0f,
                1.0f,
        };
const float Y_RANGE[]={//粒子发射的y 范围
        0.3f,
        0.3f,
        0.15f,
        0.15f,
};
const float RADIS[] ={//每个粒子的半径
                0.8f,
                0.8f,
                0.8f,
                0.8f,
};
const int COUNT[]= {//粒子数量
                340,
                340,
                99,
                99,
};
const int GROUP_COUNT[]= {//每批激活的粒子数量
                4,
                4,
                1,
                1,
};
const float LIFE_SPAN_STEP[]={//每个粒子生命周期步进
                0.07f,
                0.07f,
                0.07f,
                0.07f,
};
const float MAX_LIFE_SPAN[]={//每个粒子最大生命周期
                5.0f,
                5.0f,
                6.0f,
                6.0f,
        };
const float VY[]={//每个粒子Y 方向速度
                0.05f,
                0.05f,
                0.04f,
                0.04f,
};
const float ParticlePositon[4][2]={//粒子系统位置
        7.0f,7.0f,
        -7.0f,-7.0f,
        -7.0f,7.0f,
        7.0f,-7.0f,
};
const float START_COLOR[4][4]={//粒子的起始颜色
                {0.7569f,0.2471f,0.1176f,1.0f},//0——普通火焰
                {0.7569f,0.2471f,0.1176f,1.0f},//1——白亮火焰
                {0.6f,0.6f,0.6f,1.0f},//2——普通烟
                {0.6f,0.6f,0.6f,1.0f},//3——纯黑烟
};
const float END_COLOR[4][4]={//粒子的终止颜色
                {0.0f,0.0f,0.0f,0.0f},//0——普通火焰
                {0.0f,0.0f,0.0f,0.0f},//1——白亮火焰
                {0.0f,0.0f,0.0f,0.0f},//2——普通烟
                {0.0f,0.0f,0.0f,0.0f},//3——纯黑烟
};
