/****
DIAMOND protein aligner
Copyright (C) 2020 Max Planck Society for the Advancement of Science e.V.

Code developed by Benjamin Buchfink <benjamin.buchfink@tue.mpg.de>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
****/

#include "../standard_matrix.h"

namespace Stats {

const StandardMatrix blosum80{

    10, 1, 

	{ { (double)INT2_MAX, (double)INT2_MAX, (double)INT2_MAX, 0.3430, 0.177, 0.6568, 0.5222, -1.6, 0.564057, 1.918130, 1.918130 },
	{ 25, 2, (double)INT2_MAX, 0.342, 0.17, 0.66, 0.52, -1.6, 0.563956, 1.731000, 1.731300 },
	{ 13, 2, (double)INT2_MAX, 0.336, 0.15, 0.57, 0.59, -3, 0.570979, 2.673470, 2.692300 },
	{ 9, 2, (double)INT2_MAX, 0.319, 0.11, 0.42, 0.76, -6, 0.587837, 5.576090, 5.667860 },
	{ 8, 2, (double)INT2_MAX, 0.308, 0.090, 0.35, 0.89, -9, 0.597556, 7.536950, 7.686230 },
	{ 7, 2, (double)INT2_MAX, 0.293, 0.070, 0.27, 1.1, -14, 0.615254, 11.586600, 11.840400 },
	{ 6, 2, (double)INT2_MAX, 0.268, 0.045, 0.19, 1.4, -19, 0.644054, 19.958100, 20.441200 },
	{ 11, 1, (double)INT2_MAX, 0.314, 0.095, 0.35, 0.90, -9, 0.590702, 8.808610, 9.223320 },
	{ 10, 1, (double)INT2_MAX, 0.299, 0.071, 0.27, 1.1, -14, 0.609620, 13.833800, 14.533400 },
	{ 9, 1, (double)INT2_MAX, 0.279, 0.048, 0.20, 1.4, -19, 0.623800, 24.252000, 25.490400 } },

	{
		   /*       A,  R,  N,  D,  C,  Q,  E,  G,  H,  I,  L,  K,  M,
             F,  P,  S,  T,  W,  Y,  V,  B,  J,  Z,  X,  *        */ 
    /*A*/    5, -2, -2, -2, -1, -1, -1,  0, -2, -2, -2, -1, -1,
            -3, -1,  1,  0, -3, -2,  0, -2, -2, -1, -1, -6, -12,
    /*R*/   -2,  6, -1, -2, -4,  1, -1, -3,  0, -3, -3,  2, -2,
            -4, -2, -1, -1, -4, -3, -3, -1, -3,  0, -1, -6, -12,
    /*N*/   -2, -1,  6,  1, -3,  0, -1, -1,  0, -4, -4,  0, -3,
            -4, -3,  0,  0, -4, -3, -4,  5, -4,  0, -1, -6, -12,
    /*D*/   -2, -2,  1,  6, -4, -1,  1, -2, -2, -4, -5, -1, -4,
            -4, -2, -1, -1, -6, -4, -4,  5, -5,  1, -1, -6, -12,
    /*C*/   -1, -4, -3, -4,  9, -4, -5, -4, -4, -2, -2, -4, -2,
            -3, -4, -2, -1, -3, -3, -1, -4, -2, -4, -1, -6, -12,
    /*Q*/   -1,  1,  0, -1, -4,  6,  2, -2,  1, -3, -3,  1,  0,
            -4, -2,  0, -1, -3, -2, -3,  0, -3,  4, -1, -6, -12,
    /*E*/   -1, -1, -1,  1, -5,  2,  6, -3,  0, -4, -4,  1, -2,
            -4, -2,  0, -1, -4, -3, -3,  1, -4,  5, -1, -6, -12,
    /*G*/    0, -3, -1, -2, -4, -2, -3,  6, -3, -5, -4, -2, -4,
            -4, -3, -1, -2, -4, -4, -4, -1, -5, -3, -1, -6, -12,
    /*H*/   -2,  0,  0, -2, -4,  1,  0, -3,  8, -4, -3, -1, -2,
            -2, -3, -1, -2, -3,  2, -4, -1, -4,  0, -1, -6, -12,
    /*I*/   -2, -3, -4, -4, -2, -3, -4, -5, -4,  5,  1, -3,  1,
            -1, -4, -3, -1, -3, -2,  3, -4,  3, -4, -1, -6, -12,
    /*L*/   -2, -3, -4, -5, -2, -3, -4, -4, -3,  1,  4, -3,  2,
             0, -3, -3, -2, -2, -2,  1, -4,  3, -3, -1, -6, -12,
    /*K*/   -1,  2,  0, -1, -4,  1,  1, -2, -1, -3, -3,  5, -2,
            -4, -1, -1, -1, -4, -3, -3, -1, -3,  1, -1, -6, -12,
    /*M*/   -1, -2, -3, -4, -2,  0, -2, -4, -2,  1,  2, -2,  6,
             0, -3, -2, -1, -2, -2,  1, -3,  2, -1, -1, -6, -12,
    /*F*/   -3, -4, -4, -4, -3, -4, -4, -4, -2, -1,  0, -4,  0,
             6, -4, -3, -2,  0,  3, -1, -4,  0, -4, -1, -6, -12,
    /*P*/   -1, -2, -3, -2, -4, -2, -2, -3, -3, -4, -3, -1, -3,
            -4,  8, -1, -2, -5, -4, -3, -2, -4, -2, -1, -6, -12,
    /*S*/    1, -1,  0, -1, -2,  0,  0, -1, -1, -3, -3, -1, -2,
            -3, -1,  5,  1, -4, -2, -2,  0, -3,  0, -1, -6, -12,
    /*T*/    0, -1,  0, -1, -1, -1, -1, -2, -2, -1, -2, -1, -1,
            -2, -2,  1,  5, -4, -2,  0, -1, -1, -1, -1, -6, -12,
    /*W*/   -3, -4, -4, -6, -3, -3, -4, -4, -3, -3, -2, -4, -2,
             0, -5, -4, -4, 11,  2, -3, -5, -3, -3, -1, -6, -12,
    /*Y*/   -2, -3, -3, -4, -3, -2, -3, -4,  2, -2, -2, -3, -2,
             3, -4, -2, -2,  2,  7, -2, -3, -2, -3, -1, -6, -12,
    /*V*/    0, -3, -4, -4, -1, -3, -3, -4, -4,  3,  1, -3,  1,
            -1, -3, -2,  0, -3, -2,  4, -4,  2, -3, -1, -6, -12,
    /*B*/   -2, -1,  5,  5, -4,  0,  1, -1, -1, -4, -4, -1, -3,
            -4, -2,  0, -1, -5, -3, -4,  5, -4,  0, -1, -6, -12,
    /*J*/   -2, -3, -4, -5, -2, -3, -4, -5, -4,  3,  3, -3,  2,
             0, -4, -3, -1, -3, -2,  2, -4,  3, -3, -1, -6, -12,
    /*Z*/   -1,  0,  0,  1, -4,  4,  5, -3,  0, -4, -3,  1, -1,
            -4, -2,  0, -1, -3, -3, -3,  0, -3,  5, -1, -6, -12,
    /*X*/   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -6, -12,
    /***/   -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6,
            -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6,  1, -12,
			 -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12,
			 -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12 },

    { {2.5190801261944146e-02, 2.0340063576029794e-03, 1.5934450456787860e-03,
   1.7567882204091898e-03, 1.5342531535382758e-03, 1.7000666571091031e-03,
   2.7868757423594017e-03, 5.3312141672192219e-03, 9.4989551434358397e-04,
   2.6511420856442343e-03, 3.6482390472164553e-03, 2.8776040054767438e-03,
   1.1106592033970583e-03, 1.3579821676123010e-03, 2.1407549535065536e-03,
   6.3814552201747433e-03, 3.5676984041129148e-03, 3.2555946863964375e-04,
   1.1044305938818220e-03, 4.6043558088638440e-03},
  {2.0340063576029794e-03, 2.0957619284835711e-02, 1.6758487367421928e-03,
   1.2888167784480715e-03, 3.3826512646630144e-04, 2.3643445818728338e-03,
   2.2882006533010476e-03, 1.4590947777562337e-03, 1.1860735000568496e-03,
   1.0137588625981279e-03, 1.8206526954490602e-03, 6.0560763047425702e-03,
   6.2337540140620625e-04, 6.8229547250252861e-04, 8.5854176274829876e-04,
   2.0042877649097124e-03, 1.5124193439838248e-03, 2.1495826193954981e-04,
   7.3405490506437917e-04, 1.3053875072860433e-03},
  {1.5934450456787860e-03, 1.6758487367421928e-03, 1.6571102507116985e-02,
   3.6538256029687933e-03, 3.7275314562150083e-04, 1.3857458787672526e-03,
   1.9028611939066358e-03, 2.5087252791082940e-03, 1.2287120285654496e-03,
   7.4457563298143141e-04, 1.0685540535108611e-03, 2.2109484306704398e-03,
   4.0104114114639772e-04, 5.5233725060967317e-04, 6.5378230775305264e-04,
   2.8681766931253736e-03, 1.9573168921473445e-03, 1.3788261408849363e-04,
   5.7618561566013431e-04, 9.3483242504674316e-04},
  {1.7567882204091898e-03, 1.2888167784480715e-03, 3.6538256029687933e-03,
   2.6192308612565114e-02, 3.3106691118318270e-04, 1.3768335120404338e-03,
   4.7855109048933571e-03, 2.2232114513351937e-03, 8.1024112807968755e-04,
   7.7107432004201270e-04, 1.0524113745173926e-03, 1.9891158615195038e-03,
   3.3025592590943029e-04, 5.9228818844263222e-04, 9.2722402594094192e-04,
   2.3761553355682980e-03, 1.6423964465390574e-03, 1.1250313032631603e-04,
   4.5827153005301055e-04, 9.6057858837385150e-04},
  {1.5342531535382758e-03, 3.3826512646630144e-04, 3.7275314562150083e-04,
   3.3106691118318270e-04, 1.7242275829262476e-02, 2.8646474074708189e-04,
   2.8282275916935814e-04, 6.0199758435290119e-04, 1.6234838273827443e-04,
   1.1265997431595498e-03, 1.4150540356314203e-03, 3.8133934535800398e-04,
   3.5227309589971249e-04, 5.3668272671786679e-04, 2.9686417147468063e-04,
   9.5169015207393563e-04, 8.7056289887733830e-04, 1.2638354788184329e-04,
   3.0970164723884082e-04, 1.3402441983659027e-03},
  {1.7000666571091031e-03, 2.3643445818728338e-03, 1.3857458787672526e-03,
   1.3768335120404338e-03, 2.8646474074708189e-04, 9.4347765835274943e-03,
   3.4983547675341785e-03, 1.0949548858705770e-03, 1.1246735029182945e-03,
   6.9825772263661188e-04, 1.3621164757556699e-03, 2.8087671852638188e-03,
   7.2881923779495811e-04, 4.5182331847654853e-04, 6.9151715980429068e-04,
   1.6537440549432947e-03, 1.2213788117405877e-03, 1.9879174677422644e-04,
   5.4136767114461271e-04, 1.0117238998697397e-03},
  {2.7868757423594017e-03, 2.2882006533010476e-03, 1.9028611939066358e-03,
   4.7855109048933571e-03, 2.8282275916935814e-04, 3.4983547675341785e-03,
   2.0820161672110919e-02, 1.6705768658803739e-03, 1.2497919872780897e-03,
   9.6840513155216357e-04, 1.4960914225340589e-03, 3.5721908719380593e-03,
   5.7149419172049785e-04, 6.3988473796368903e-04, 1.2122196186289647e-03,
   2.6389347134212079e-03, 1.8749387967046983e-03, 1.9036380224872525e-04,
   6.3290036404780182e-04, 1.4751061012869160e-03},
  {5.3312141672192219e-03, 1.4590947777562337e-03, 2.5087252791082940e-03,
   2.2232114513351937e-03, 6.0199758435290119e-04, 1.0949548858705770e-03,
   1.6705768658803739e-03, 4.6339669111671353e-02, 7.5449961042330647e-04,
   9.4996724611759150e-04, 1.6066592192185663e-03, 2.0310505314366269e-03,
   5.3609624387219209e-04, 8.9864424405597940e-04, 1.0176342727297918e-03,
   3.4421697217907042e-03, 1.8920241991862482e-03, 2.9373791998702517e-04,
   6.1327956332154923e-04, 1.4084342214988788e-03},
  {9.4989551434358397e-04, 1.1860735000568496e-03, 1.2287120285654496e-03,
   8.1024112807968755e-04, 1.6234838273827443e-04, 1.1246735029182945e-03,
   1.2497919872780897e-03, 7.5449961042330647e-04, 1.0381617825381334e-02,
   4.4036371570812095e-04, 7.9508382719793930e-04, 1.0307766502203480e-03,
   2.6842308809614442e-04, 6.8472295590456679e-04, 4.0762982846324675e-04,
   9.6226821486916257e-04, 6.8751986685796880e-04, 1.4371608217276840e-04,
   1.6115232208711219e-03, 5.3755732656986873e-04},
  {2.6511420856442343e-03, 1.0137588625981279e-03, 7.4457563298143141e-04,
   7.7107432004201270e-04, 1.1265997431595498e-03, 6.9825772263661188e-04,
   9.6840513155216357e-04, 9.4996724611759150e-04, 4.4036371570812095e-04,
   2.1961426293375986e-02, 1.1130703369690390e-02, 1.1532712305851239e-03,
   2.4830606782385335e-03, 2.6597759616518901e-03, 7.3360010098323712e-04,
   1.4571406164888326e-03, 2.3588073239766052e-03, 3.3397592349852914e-04,
   1.2624333898651637e-03, 1.2271122820238527e-02},
  {3.6482390472164553e-03, 1.8206526954490602e-03, 1.0685540535108611e-03,
   1.0524113745173926e-03, 1.4150540356314203e-03, 1.3621164757556699e-03,
   1.4960914225340589e-03, 1.6066592192185663e-03, 7.9508382719793930e-04,
   1.1130703369690390e-02, 4.4208543625750947e-02, 1.9460828295282577e-03,
   5.1637253946183711e-03, 5.2243696624605148e-03, 1.1528201208432873e-03,
   2.0991567569779157e-03, 2.7981964830167271e-03, 6.3277768036502599e-04,
   2.0134515231625665e-03, 8.8913964764739482e-03},
  {2.8776040054767438e-03, 6.0560763047425702e-03, 2.2109484306704398e-03,
   1.9891158615195038e-03, 3.8133934535800398e-04, 2.8087671852638188e-03,
   3.5721908719380593e-03, 2.0310505314366269e-03, 1.0307766502203480e-03,
   1.1532712305851239e-03, 1.9460828295282577e-03, 1.8992311051287718e-02,
   7.1570376147636942e-04, 7.2977429729635017e-04, 1.2494764855845110e-03,
   2.5724600423967150e-03, 2.0209702186968510e-03, 1.9168209136920497e-04,
   7.8006357431686252e-04, 1.4847749228956784e-03},
  {1.1106592033970583e-03, 6.2337540140620625e-04, 4.0104114114639772e-04,
   3.3025592590943029e-04, 3.5227309589971249e-04, 7.2881923779495811e-04,
   5.7149419172049785e-04, 5.3609624387219209e-04, 2.6842308809614442e-04,
   2.4830606782385335e-03, 5.1637253946183711e-03, 7.1570376147636942e-04,
   5.3068580972096564e-03, 1.0285774291964053e-03, 3.3805426284931307e-04,
   6.9732452132326552e-04, 9.2870717652810894e-04, 1.9855470215781828e-04,
   4.6842552580683944e-04, 2.1900560317616967e-03},
  {1.3579821676123010e-03, 6.8229547250252861e-04, 5.5233725060967317e-04,
   5.9228818844263222e-04, 5.3668272671786679e-04, 4.5182331847654853e-04,
   6.3988473796368903e-04, 8.9864424405597940e-04, 6.8472295590456679e-04,
   2.6597759616518901e-03, 5.2243696624605148e-03, 7.2977429729635017e-04,
   1.0285774291964053e-03, 2.1050081047094053e-02, 4.2695305789420819e-04,
   9.9526342159352650e-04, 1.0513746746114907e-03, 7.4360162788489814e-04,
   4.5643669065202081e-03, 2.2374943285650862e-03},
  {2.1407549535065536e-03, 8.5854176274829876e-04, 6.5378230775305264e-04,
   9.2722402594094192e-04, 2.9686417147468063e-04, 6.9151715980429068e-04,
   1.2122196186289647e-03, 1.0176342727297918e-03, 4.0762982846324675e-04,
   7.3360010098323712e-04, 1.1528201208432873e-03, 1.2494764855845110e-03,
   3.3805426284931307e-04, 4.2695305789420819e-04, 2.2132147568842062e-02,
   1.4255613382498733e-03, 1.0735194808918437e-03, 9.8582208625861730e-05,
   3.4299989987846913e-04, 1.0356422797820938e-03},
  {6.3814552201747433e-03, 2.0042877649097124e-03, 2.8681766931253736e-03,
   2.3761553355682980e-03, 9.5169015207393563e-04, 1.6537440549432947e-03,
   2.6389347134212079e-03, 3.4421697217907042e-03, 9.6226821486916257e-04,
   1.4571406164888326e-03, 2.0991567569779157e-03, 2.5724600423967150e-03,
   6.9732452132326552e-04, 9.9526342159352650e-04, 1.4255613382498733e-03,
   1.6728326838562371e-02, 4.7707748874885403e-03, 2.2457429196472549e-04,
   9.2159836957901727e-04, 2.0684030169239110e-03},
  {3.5676984041129148e-03, 1.5124193439838248e-03, 1.9573168921473445e-03,
   1.6423964465390574e-03, 8.7056289887733830e-04, 1.2213788117405877e-03,
   1.8749387967046983e-03, 1.8920241991862482e-03, 6.8751986685796880e-04,
   2.3588073239766052e-03, 2.7981964830167271e-03, 2.0209702186968510e-03,
   9.2870717652810894e-04, 1.0513746746114907e-03, 1.0735194808918437e-03,
   4.7707748874885403e-03, 1.5594999664688735e-02, 2.0707952088013908e-04,
   8.2888908190237710e-04, 3.2712773918631040e-03},
  {3.2555946863964375e-04, 2.1495826193954981e-04, 1.3788261408849363e-04,
   1.1250313032631603e-04, 1.2638354788184329e-04, 1.9879174677422644e-04,
   1.9036380224872525e-04, 2.9373791998702517e-04, 1.4371608217276840e-04,
   3.3397592349852914e-04, 6.3277768036502599e-04, 1.9168209136920497e-04,
   1.9855470215781828e-04, 7.4360162788489814e-04, 9.8582208625861730e-05,
   2.2457429196472549e-04, 2.0707952088013908e-04, 8.7238920952867408e-03,
   1.0281252180987448e-03, 3.6291275126003661e-04},
  {1.1044305938818220e-03, 7.3405490506437917e-04, 5.7618561566013431e-04,
   4.5827153005301055e-04, 3.0970164723884082e-04, 5.4136767114461271e-04,
   6.3290036404780182e-04, 6.1327956332154923e-04, 1.6115232208711219e-03,
   1.2624333898651637e-03, 2.0134515231625665e-03, 7.8006357431686252e-04,
   4.6842552580683944e-04, 4.5643669065202081e-03, 3.4299989987846913e-04,
   9.2159836957901727e-04, 8.2888908190237710e-04, 1.0281252180987448e-03,
   1.4809695895202394e-02, 1.2479627747244684e-03},
  {4.6043558088638440e-03, 1.3053875072860433e-03, 9.3483242504674316e-04,
   9.6057858837385150e-04, 1.3402441983659027e-03, 1.0117238998697397e-03,
   1.4751061012869160e-03, 1.4084342214988788e-03, 5.3755732656986873e-04,
   1.2271122820238527e-02, 8.8913964764739482e-03, 1.4847749228956784e-03,
   2.1900560317616967e-03, 2.2374943285650862e-03, 1.0356422797820938e-03,
   2.0684030169239110e-03, 3.2712773918631040e-03, 3.6291275126003661e-04,
   1.2479627747244684e-03, 2.4557979462295820e-02} },

   { 7.2647227078731003e-02, 5.0418078079712533e-02, 4.2998652475215837e-02,
 5.3630877849155469e-02, 2.8859643195758448e-02, 3.3634522394591609e-02,
 5.4557686298480158e-02, 7.6673641116832586e-02, 2.5417438256716121e-02,
 6.7169462169032662e-02, 9.9526086073919384e-02, 5.4794439692059750e-02,
 2.4441485110408976e-02, 4.7108293477054419e-02, 3.8215524905474578e-02,
 5.7239465972425123e-02, 5.0130851564694504e-02, 1.4489654685450317e-02,
 3.4849727270340386e-02, 7.3197242333946158e-02 },

  { {0.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00,
0.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00,
0.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00,
0.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00,
0.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00,
0.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00,
0.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00},
{0.00000000e+00, 4.77313697e+00, 4.77250219e-01, 7.31790935e-01,
 4.50905428e-01, 7.03141318e-01, 3.96805472e-01, 9.57108118e-01,
 5.14428496e-01, 5.43302985e-01, 7.22895459e-01, 5.04576848e-01,
 6.25509899e-01, 5.10109254e-01, 7.71095354e-01, 6.95763501e-01,
 5.55324683e-01, 1.53463504e+00, 9.79634362e-01, 8.65874830e-01,
 3.09281009e-01, 7.50000000e-01, 4.36234494e-01, 7.00327585e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 5.20181427e-01},
{0.00000000e+00, 4.77250219e-01, 5.36249708e+00, 2.52383415e-01,
 5.75921529e+00, 1.26868736e+00, 2.51452613e-01, 6.38679494e-01,
 8.30167261e-01, 2.33516284e-01, 7.93249084e-01, 2.20539708e-01,
 3.09639510e-01, 4.86768283e+00, 4.28138134e-01, 8.50001386e-01,
 6.08526572e-01, 9.48166786e-01, 7.43109762e-01, 2.67977796e-01,
 1.78830543e-01, 7.50000000e-01, 3.07187296e-01, 1.10900996e+00,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 2.25768580e-01},
{0.00000000e+00, 7.31790935e-01, 2.52383415e-01, 2.07020176e+01,
 2.13899571e-01, 1.79625278e-01, 3.94756499e-01, 2.72055621e-01,
 2.21322329e-01, 5.81174860e-01, 2.41148280e-01, 4.92657548e-01,
 4.99414247e-01, 3.00383113e-01, 2.69170214e-01, 2.95117493e-01,
 2.32476995e-01, 5.76114817e-01, 6.01733396e-01, 6.34451252e-01,
 3.02232769e-01, 7.50000000e-01, 3.07930831e-01, 2.23671408e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 5.28325329e-01},
{0.00000000e+00, 4.50905428e-01, 5.75921529e+00, 2.13899571e-01,
 9.10634223e+00, 1.63552603e+00, 2.34434106e-01, 5.40654444e-01,
 5.94384672e-01, 2.14047157e-01, 6.76875328e-01, 1.97166761e-01,
 2.51946380e-01, 1.58444827e+00, 4.52407652e-01, 7.63275386e-01,
 4.76639455e-01, 7.74041684e-01, 6.10882957e-01, 2.44693975e-01,
 1.44774364e-01, 7.50000000e-01, 2.45193294e-01, 1.30286928e+00,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 2.03968665e-01},
{0.00000000e+00, 7.03141318e-01, 1.26868736e+00, 1.79625278e-01,
 1.63552603e+00, 6.99475019e+00, 2.48970812e-01, 3.99359878e-01,
 9.01259830e-01, 2.64258683e-01, 1.19492933e+00, 2.75527698e-01,
 4.28576433e-01, 8.11140936e-01, 5.81414067e-01, 1.90643780e+00,
 8.31863127e-01, 8.45039731e-01, 6.85529376e-01, 3.69379335e-01,
 2.40807712e-01, 7.50000000e-01, 3.32874109e-01, 5.05418243e+00,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 2.70986883e-01},
{0.00000000e+00, 3.96805472e-01, 2.51452613e-01, 3.94756499e-01,
 2.34434106e-01, 2.48970812e-01, 9.48547379e+00, 2.48796540e-01,
 5.71854758e-01, 8.40573731e-01, 2.82718808e-01, 1.11429351e+00,
 8.93330223e-01, 2.72679266e-01, 2.37160811e-01, 2.85158291e-01,
 2.87269045e-01, 3.69100895e-01, 4.45199900e-01, 6.48888163e-01,
 1.08939386e+00, 7.50000000e-01, 2.78024963e+00, 2.62771902e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 1.00399896e+00},
{0.00000000e+00, 9.57108118e-01, 6.38679494e-01, 2.72055621e-01,
 5.40654444e-01, 3.99359878e-01, 2.48796540e-01, 7.88244144e+00,
 3.87151686e-01, 1.84455102e-01, 4.83435068e-01, 2.10542976e-01,
 2.86067872e-01, 7.60943081e-01, 3.47300820e-01, 4.24585221e-01,
 3.77442786e-01, 7.84315177e-01, 4.92238430e-01, 2.50954947e-01,
 2.64396616e-01, 7.50000000e-01, 2.29516029e-01, 4.08980256e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 2.00030947e-01},
{0.00000000e+00, 5.14428496e-01, 8.30167261e-01, 2.21322329e-01,
 5.94384672e-01, 9.01259830e-01, 5.71854758e-01, 3.87151686e-01,
 1.60694674e+01, 2.57933576e-01, 7.40110055e-01, 3.14299881e-01,
 4.32076355e-01, 1.12425153e+00, 4.19656882e-01, 1.31555625e+00,
 9.25536475e-01, 6.61407005e-01, 5.39570743e-01, 2.88933760e-01,
 3.90225420e-01, 7.50000000e-01, 1.81930455e+00, 1.05926315e+00,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 2.91587251e-01},
{0.00000000e+00, 5.43302985e-01, 2.33516284e-01, 5.81174860e-01,
 2.14047157e-01, 2.64258683e-01, 8.40573731e-01, 1.84455102e-01,
 2.57933576e-01, 4.86762150e+00, 3.13345237e-01, 1.66499837e+00,
 1.51247384e+00, 2.57799519e-01, 2.85790430e-01, 3.09071252e-01,
 2.99348100e-01, 3.78995471e-01, 7.00511896e-01, 2.49584558e+00,
 3.43150987e-01, 7.50000000e-01, 5.39308441e-01, 2.81349188e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 2.95548558e+00},
{0.00000000e+00, 7.22895459e-01, 7.93249084e-01, 2.41148280e-01,
 6.76875328e-01, 1.19492933e+00, 2.82718808e-01, 4.83435068e-01,
 7.40110055e-01, 3.13345237e-01, 6.32564527e+00, 3.56851811e-01,
 5.34403407e-01, 9.38398440e-01, 5.96694133e-01, 1.52403165e+00,
 2.19214139e+00, 8.20193974e-01, 7.35729790e-01, 3.70194033e-01,
 2.41427653e-01, 7.50000000e-01, 4.08501941e-01, 1.32044155e+00,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 3.39320970e-01},
{0.00000000e+00, 5.04576848e-01, 2.20539708e-01, 4.92657548e-01,
 1.97166761e-01, 2.75527698e-01, 1.11429351e+00, 2.10542976e-01,
 3.14299881e-01, 1.66499837e+00, 3.56851811e-01, 4.46305621e+00,
 2.12274889e+00, 2.49692056e-01, 3.03099202e-01, 4.06904090e-01,
 3.62830591e-01, 3.68478686e-01, 5.60836408e-01, 1.22050154e+00,
 4.38789464e-01, 7.50000000e-01, 5.80503535e-01, 3.25631695e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 3.33558735e+00},
{0.00000000e+00, 6.25509899e-01, 3.09639510e-01, 4.99414247e-01,
 2.51946380e-01, 4.28576433e-01, 8.93330223e-01, 2.86067872e-01,
 4.32076355e-01, 1.51247384e+00, 5.34403407e-01, 2.12274889e+00,
 8.88346290e+00, 3.81598352e-01, 3.61925345e-01, 8.86557630e-01,
 5.05866341e-01, 4.98438721e-01, 7.57959723e-01, 1.22414515e+00,
 5.60653516e-01, 7.50000000e-01, 5.49937808e-01, 6.03240148e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 1.87684042e+00},
{0.00000000e+00, 5.10109254e-01, 4.86768283e+00, 3.00383113e-01,
 1.58444827e+00, 8.11140936e-01, 2.72679266e-01, 7.60943081e-01,
 1.12425153e+00, 2.57799519e-01, 9.38398440e-01, 2.49692056e-01,
 3.81598352e-01, 8.96275887e+00, 3.97867522e-01, 9.58172021e-01,
 7.73025258e-01, 1.16534759e+00, 9.08032132e-01, 2.97018980e-01,
 2.21307752e-01, 7.50000000e-01, 3.84510481e-01, 8.67215281e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 2.52958933e-01},
{0.00000000e+00, 7.71095354e-01, 4.28138134e-01, 2.69170214e-01,
 4.52407652e-01, 5.81414067e-01, 2.37160811e-01, 3.47300820e-01,
 4.19656882e-01, 2.85790430e-01, 5.96694133e-01, 3.03099202e-01,
 3.61925345e-01, 3.97867522e-01, 1.51545798e+01, 5.37994539e-01,
 4.45589871e-01, 6.51704179e-01, 5.60357280e-01, 3.70233083e-01,
 1.78033069e-01, 7.50000000e-01, 2.57545983e-01, 5.64854837e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 2.96124685e-01},
{0.00000000e+00, 6.95763501e-01, 8.50001386e-01, 2.95117493e-01,
 7.63275386e-01, 1.90643780e+00, 2.85158291e-01, 4.24585221e-01,
 1.31555625e+00, 3.09071252e-01, 1.52403165e+00, 4.06904090e-01,
 8.86557630e-01, 9.58172021e-01, 5.37994539e-01, 8.33990474e+00,
 1.39424540e+00, 8.58988713e-01, 7.24369298e-01, 4.10943414e-01,
 4.07901262e-01, 7.50000000e-01, 4.61857147e-01, 4.36001721e+00,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 3.67482647e-01},
{0.00000000e+00, 5.55324683e-01, 6.08526572e-01, 2.32476995e-01,
 4.76639455e-01, 8.31863127e-01, 2.87269045e-01, 3.77442786e-01,
 9.25536475e-01, 2.99348100e-01, 2.19214139e+00, 3.62830591e-01,
 5.05866341e-01, 7.73025258e-01, 4.45589871e-01, 1.39424540e+00,
 8.24459589e+00, 6.94509540e-01, 5.98385216e-01, 3.53719047e-01,
 2.94245493e-01, 7.50000000e-01, 4.17775411e-01, 1.04634306e+00,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 3.37250515e-01},
{0.00000000e+00, 1.53463504e+00, 9.48166786e-01, 5.76114817e-01,
 7.74041684e-01, 8.45039731e-01, 3.69100895e-01, 7.84315177e-01,
 6.61407005e-01, 3.78995471e-01, 8.20193974e-01, 3.68478686e-01,
 4.98438721e-01, 1.16534759e+00, 6.51704179e-01, 8.58988713e-01,
 6.94509540e-01, 5.10577131e+00, 1.66260189e+00, 4.93679246e-01,
 2.70773669e-01, 7.50000000e-01, 4.62005069e-01, 8.50359559e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 3.72716393e-01},
{0.00000000e+00, 9.79634362e-01, 7.43109762e-01, 6.01733396e-01,
 6.10882957e-01, 6.85529376e-01, 4.45199900e-01, 4.92238430e-01,
 5.39570743e-01, 7.00511896e-01, 7.35729790e-01, 5.60836408e-01,
 7.57959723e-01, 9.08032132e-01, 5.60357280e-01, 7.24369298e-01,
 5.98385216e-01, 1.66260189e+00, 6.20547751e+00, 8.91492247e-01,
 2.85084781e-01, 7.50000000e-01, 4.74451641e-01, 7.00342047e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 6.17118219e-01},
{0.00000000e+00, 8.65874830e-01, 2.67977796e-01, 6.34451252e-01,
 2.44693975e-01, 3.69379335e-01, 6.48888163e-01, 2.50954947e-01,
 2.88933760e-01, 2.49584558e+00, 3.70194033e-01, 1.22050154e+00,
 1.22414515e+00, 2.97018980e-01, 3.70233083e-01, 4.10943414e-01,
 3.53719047e-01, 4.93679246e-01, 8.91492247e-01, 4.58356287e+00,
 3.42175943e-01, 7.50000000e-01, 4.89223745e-01, 3.85230939e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 1.73439753e+00},
{0.00000000e+00, 3.09281009e-01, 1.78830543e-01, 3.02232769e-01,
 1.44774364e-01, 2.40807712e-01, 1.08939386e+00, 2.64396616e-01,
 3.90225420e-01, 3.43150987e-01, 2.41427653e-01, 4.38789464e-01,
 5.60653516e-01, 2.21307752e-01, 1.78033069e-01, 4.07901262e-01,
 2.94245493e-01, 2.70773669e-01, 2.85084781e-01, 3.42175943e-01,
 4.15522183e+01, 7.50000000e-01, 2.03605072e+00, 3.04533429e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 4.00252232e-01},
{0.00000000e+00, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 7.50000000e-01},
{0.00000000e+00, 4.36234494e-01, 3.07187296e-01, 3.07930831e-01,
 2.45193294e-01, 3.32874109e-01, 2.78024963e+00, 2.29516029e-01,
 1.81930455e+00, 5.39308441e-01, 4.08501941e-01, 5.80503535e-01,
 5.49937808e-01, 3.84510481e-01, 2.57545983e-01, 4.61857147e-01,
 4.17775411e-01, 4.62005069e-01, 4.74451641e-01, 4.89223745e-01,
 2.03605072e+00, 7.50000000e-01, 1.21940332e+01, 3.82065335e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 5.63904098e-01},
{0.00000000e+00, 7.00327585e-01, 1.10900996e+00, 2.23671408e-01,
 1.30286928e+00, 5.05418243e+00, 2.62771902e-01, 4.08980256e-01,
 1.05926315e+00, 2.81349188e-01, 1.32044155e+00, 3.25631695e-01,
 6.03240148e-01, 8.67215281e-01, 5.64854837e-01, 4.36001721e+00,
 1.04634306e+00, 8.50359559e-01, 7.00342047e-01, 3.85230939e-01,
 3.04533429e-01, 7.50000000e-01, 3.82065335e-01, 4.78944345e+00,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 3.07788194e-01},
{0.00000000e+00, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 7.50000000e-01},
{0.00000000e+00, 1.40000000e-01, 1.40000000e-01, 1.40000000e-01,
 1.40000000e-01, 1.40000000e-01, 1.40000000e-01, 1.40000000e-01,
 1.40000000e-01, 1.40000000e-01, 1.40000000e-01, 1.40000000e-01,
 1.40000000e-01, 1.40000000e-01, 1.40000000e-01, 1.40000000e-01,
 1.40000000e-01, 1.40000000e-01, 1.40000000e-01, 1.40000000e-01,
 1.40000000e-01, 1.40000000e-01, 1.40000000e-01, 1.40000000e-01,
 1.40000000e-01, 1.33300000e+00, 1.40000000e-01, 1.40000000e-01},
{0.00000000e+00, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 7.50000000e-01, 7.50000000e-01, 7.50000000e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 7.50000000e-01},
{0.00000000e+00, 5.20181427e-01, 2.25768580e-01, 5.28325329e-01,
 2.03968665e-01, 2.70986883e-01, 1.00399896e+00, 2.00030947e-01,
 2.91587251e-01, 2.95548558e+00, 3.39320970e-01, 3.33558735e+00,
 1.87684042e+00, 2.52958933e-01, 2.96124685e-01, 3.67482647e-01,
 3.37250515e-01, 3.72716393e-01, 6.17118219e-01, 1.73439753e+00,
 4.00252232e-01, 7.50000000e-01, 5.63904098e-01, 3.07788194e-01,
 7.50000000e-01, 1.40000000e-01, 7.50000000e-01, 3.18242650e+00} }

};

}