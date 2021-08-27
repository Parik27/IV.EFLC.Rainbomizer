#include "Utils.hh"

int (*scanf__efc7c) (char *str, char *format, ...);

class TimecycleRandomizer
{
    static int
    RandomizeTimecycle (
        char *str, char *format, int *field_0, int *field_1, int *field_2,
        int *field_3, int *field_4, int *field_5, int *field_6, int *field_7,
        int *field_8, int *field_9, int *field_10, int *field_11, int *field_12,
        int *field_13, int *field_14, int *field_15, int *field_16,
        int *field_17, int *field_18, int *field_19, int *field_20,
        float *field_21, float *field_22, float *field_23, float *field_24,
        int *field_25, int *field_26, int *field_27, int *field_28,
        int *field_29, int *field_30, int *field_31, int *field_32,
        int *field_33, int *field_34, float *field_35, float *field_36,
        float *field_37, float *field_38, int *field_39, int *field_40,
        int *field_41, int *field_42, int *field_43, int *field_44,
        float *field_45, float *field_46, float *field_47, float *field_48,
        float *field_49, float *field_50, float *field_51, float *field_52,
        float *field_53, float *field_54, float *field_55, int *field_56,
        float *field_57, float *field_58, float *field_59, float *field_60,
        float *field_61, int *field_62, float *field_63, float *field_64,
        float *field_65, float *field_66, float *field_67, float *field_68,
        float *field_69, float *field_70, float *field_71, float *field_72,
        float *field_73, float *field_74, float *field_75, float *field_76,
        float *field_77, float *field_78, float *field_79, float *field_80,
        float *field_81, float *field_82, float *field_83, float *field_84,
        float *field_85, float *field_86, float *field_87, float *field_88,
        float *field_89, float *field_90, float *field_91, float *field_92,
        float *field_93, float *field_94, float *field_95, float *field_96,
        float *field_97, float *field_98, float *field_99, float *field_100,
        float *field_101, float *field_102, float *field_103, float *field_104,
        float *field_105, float *field_106, float *field_107, float *field_108,
        float *field_109, float *field_110, float *field_111, float *field_112,
        float *field_113, float *field_114, float *field_115, float *field_116,
        float *field_117, float *field_118, float *field_119, float *field_120,
        float *field_121, float *field_122, float *field_123, float *field_124,
        float *field_125, float *field_126, float *field_127, float *field_128,
        float *field_129, float *field_130, float *field_131, float *field_132,
        float *field_133)
    {
        *field_0   = RandomInt (0, 255);
        *field_1   = RandomInt (137, 255);
        *field_2   = RandomInt (21, 255);
        *field_3   = RandomInt (0, 255);
        *field_4   = RandomInt (119, 255);
        *field_5   = RandomInt (0, 255);
        *field_6   = RandomInt (0, 255);
        *field_7   = RandomInt (0, 255);
        *field_8   = RandomInt (0, 255);
        *field_9   = RandomInt (0, 0);
        *field_10  = RandomInt (0, 0);
        *field_11  = RandomInt (0, 218);
        *field_12  = RandomInt (0, 255);
        *field_13  = RandomInt (0, 255);
        *field_14  = RandomInt (0, 255);
        *field_15  = RandomInt (0, 255);
        *field_16  = RandomInt (0, 255);
        *field_17  = RandomInt (0, 255);
        *field_18  = RandomInt (0, 255);
        *field_19  = RandomInt (0, 255);
        *field_20  = RandomInt (0, 255);
        *field_21  = RandomFloat (0.6, 2.4);
        *field_22  = RandomFloat (0.6, 2.4);
        *field_23  = RandomFloat (600, 1500);
        *field_24  = RandomFloat (9, 147.1);
        *field_25  = RandomInt (35, 120);
        *field_26  = RandomInt (9, 120);
        *field_27  = RandomInt (10, 141);
        *field_28  = RandomInt (15, 199);
        *field_29  = RandomInt (11, 230);
        *field_30  = RandomInt (34, 255);
        *field_31  = RandomInt (0, 99);
        *field_32  = RandomInt (0, 108);
        *field_33  = RandomInt (0, 138);
        *field_34  = RandomInt (50, 240);
        *field_35  = RandomFloat (0.1, 3.7);
        *field_36  = RandomFloat (0, 1.86);
        *field_37  = RandomFloat (0.19, 0.98);
        *field_38  = RandomFloat (0, 10);
        *field_39  = RandomInt (78, 155);
        *field_40  = RandomInt (88, 181);
        *field_41  = RandomInt (60, 186);
        *field_42  = RandomInt (0, 173);
        *field_43  = RandomInt (0, 222);
        *field_44  = RandomInt (0, 255);
        *field_45  = RandomFloat (0.16, 1);
        *field_46  = RandomFloat (1, 10);
        *field_47  = RandomFloat (0.6, 1.25);
        *field_48  = RandomFloat (0, 1);
        *field_49  = RandomFloat (1, 3.25);
        *field_50  = RandomFloat (0.35, 2);
        *field_51  = RandomFloat (8, 128);
        *field_52  = RandomFloat (64, 348.75);
        *field_53  = RandomFloat (0.28, 20.23);
        *field_54  = RandomFloat (176.13, 256);
        *field_55  = RandomFloat (0, 0.2);
        *field_56  = RandomInt (0, 217);
        *field_57  = RandomFloat (0, 46);
        *field_58  = RandomFloat (0.8, 22);
        *field_59  = RandomFloat (0.6, 22);
        *field_60  = RandomFloat (0.05, 19.5);
        *field_61  = RandomFloat (0, 18);
        *field_62  = RandomInt (-12, 15);
        *field_63  = RandomFloat (0.03, 2);
        *field_64  = RandomFloat (0, 1);
        *field_65  = RandomFloat (0.22, 0.929);
        *field_66  = RandomFloat (0.22, 1);
        *field_67  = RandomFloat (0, 1);
        *field_68  = RandomFloat (0.118, 0.969);
        *field_69  = RandomFloat (0.059, 1);
        *field_70  = RandomFloat (0, 1);
        *field_71  = RandomFloat (0, 0.894);
        *field_72  = RandomFloat (0, 1);
        *field_73  = RandomFloat (0, 1);
        *field_74  = RandomFloat (0.208, 0.902);
        *field_75  = RandomFloat (0, 1);
        *field_76  = RandomFloat (1, 1);
        *field_77  = RandomFloat (1.298, 16.66);
        *field_78  = RandomFloat (0, 1);
        *field_79  = RandomFloat (1, 1);
        *field_80  = RandomFloat (0, 0);
        *field_81  = RandomFloat (0.2, 0.8);
        *field_82  = RandomFloat (0.2, 0.8);
        *field_83  = RandomFloat (0.2, 0.8);
        *field_84  = RandomFloat (0.102, 1);
        *field_85  = RandomFloat (0, 1.434);
        *field_86  = RandomFloat (0, 1.07);
        *field_87  = RandomFloat (0.132, 0.312);
        *field_88  = RandomFloat (0.68, 0.68);
        *field_89  = RandomFloat (0, 1);
        *field_90  = RandomFloat (6.272, 8);
        *field_91  = RandomFloat (0, 0.837);
        *field_92  = RandomFloat (0.365, 1.7);
        *field_93  = RandomFloat (1, 1);
        *field_94  = RandomFloat (0.026, 0.754);
        *field_95  = RandomFloat (0.12, 1.513);
        *field_96  = RandomFloat (0, 1.112);
        *field_97  = RandomFloat (0, 1.58);
        *field_98  = RandomFloat (0.435, 1.265);
        *field_99  = RandomFloat (0, 0.89);
        *field_100 = RandomFloat (0, 0.89);
        *field_101 = RandomFloat (0, 0.89);
        *field_102 = RandomFloat (0, 1.265);
        *field_103 = RandomFloat (0.239, 0.953);
        *field_104 = RandomFloat (0.196, 0.922);
        *field_105 = RandomFloat (0.196, 0.922);
        *field_106 = RandomFloat (0, 1);
        *field_107 = RandomFloat (1.7, 100);
        *field_108 = RandomFloat (0, 0.484);
        *field_109 = RandomFloat (0, 0.685);
        *field_110 = RandomFloat (0, 0.256);
        *field_111 = RandomFloat (0, 25.75);
        *field_112 = RandomFloat (0, 25);
        *field_113 = RandomFloat (0, 26);
        *field_114 = RandomFloat (0, 0.199);
        *field_115 = RandomFloat (0, 4);
        *field_116 = RandomFloat (0.98, 0.98);
        *field_117 = RandomFloat (1, 1);
        *field_118 = RandomFloat (0, 0.062);
        *field_119 = RandomFloat (0, 2);
        *field_120 = RandomFloat (25, 1000);
        *field_121 = RandomFloat (0, 0);
        *field_122 = RandomFloat (0, 0);
        *field_123 = RandomFloat (0, 0.475);
        *field_124 = RandomFloat (0, 1);
        *field_125 = RandomFloat (1, 4.65);
        *field_126 = RandomFloat (0.5, 1.5);
        *field_127 = RandomFloat (1, 2);
        *field_128 = RandomFloat (0.999, 1);
        *field_129 = RandomFloat (0.55, 1);
        *field_130 = RandomFloat (0, 1);
        *field_131 = RandomFloat (0.2, 2);
        *field_132 = RandomFloat (1, 2);
        *field_133 = RandomFloat (1, 1);
        return 134;
    }

public:
    TimecycleRandomizer ()
    {

        RegisterHook (
            ByVersion ("e8 ? ? ? ? 0f b6 84 ? ? ? ? ? 0f b6 8c ? ? ? ? ? "
                       "0f b6 94 ? ? ? ? ?",
                       "e8 ? ? ? ? 0f b6 8c ? ? ? ? ? 0f b6 84 ? ? ? ? ? "
                       "81 c9 00 ff ff ff "),
            0, scanf__efc7c, RandomizeTimecycle);
    }
} tmcycrndmzr;
