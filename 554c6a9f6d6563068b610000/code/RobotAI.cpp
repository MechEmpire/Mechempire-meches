#include "RobotAI.h"
#include <iostream>
#include <time.h>

RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}

int frames = 0;//È«ÊÇ¼ÆÊý±äÁ¿...ÔÚµ×ÏÂÊ¹ÓÃµÄÊ±ºò»áÓÐ×¢ÊÍ
int run = 0;
int change[3] = { 0, 0, 0 };
double approach_angle;
bool is_approaching = true;
int change1 = 0;
int TARGET_ENGINE = 0;
int TARGET_WEAPON = 0;
int TARGET_HP = 0;
double target_velocity[10][2];
int target_velocity_num = 0;
bool is_circle_move = false;
int is_circle_move_time_out = 0;
bool not_move = true;

double** UFO_route1;
double*** bullet_position1;

bool circle_test(const double my_x, const double my_y, const double dis_x, const double dis_y)
{
	double x = my_x;
	double y = my_y;

	double x_min = my_x;
	double y_min = my_y;

	for (int i = 0; i < 1000; i++)//°ÑÕûÌõÏßÂ··Ö³É1000¶Î
	{
		x += dis_x / 1000;//Ã¿´ÎÔö¼ÓÎ»ÒÆ/1000
		y += dis_y / 1000;

		if (pow(x - 300, 2) + pow(y - 250, 2) < pow(75, 2)){ //300,250ÊÇÕÏ°­Îï×ø±ê£¬°ë¾¶ÊÇ75£¬ÕâÀïÈ¡76¿¼ÂÇµ½Îó²î£¨76²»Ò»¶¨ºÃ£¬76ÊÇÕë¶Ô¹âÀâÊµÑéµÃ³öµÄÊýÖµ£©
			return true;
		}
		if (pow(x - 1066, 2) + pow(y - 430, 2) < pow(75, 2)){ //ÁíÒ»¸öÕÏ°­Îï
			return true;
		}
	}

	return false; //²»¹ÜÄÄ¸öÎªtrue¶¼²»·¢Éä£¬¶¼Îªfalse·¢Éä
}

bool effective_range(const double my_x, const double my_y, const double dis_x, const double dis_y, double &least_distance)//¼ÆËãÊÇ·ñÔÚÓÐÐ§·¶Î§£¨´ò²»µ½Öù×ÓÉÏ£© ²ÎÊýÊÇ×Ô¼ºµÄx,y ºÍÄ¿±ê¾à×Ô¼ºµÄx,y £¨»°ËµÒÔÇ°µÄeffective_range¿ÉÄÜÓÐ´í£©
{
	double x = my_x;
	double y = my_y;
	bool is_on_way = false;//Èç¹û´òµ½Öù×ÓÉÏÄÇÃ´Õâ¸ö±äÁ¿¾ÍÎªtrue
	bool is_on_half_way[2];//Èç¹ûÄÜ´ÓÖù×ÓÖÐ¼ä´©¹ýÁ½¸ö±äÁ¿¶¼µÃÎªfalse

	is_on_half_way[0] = false;
	is_on_half_way[1] = false;

	double x_min = my_x;
	double y_min = my_y;

	for (int i = 0; i < 1000; i++)//°ÑÕûÌõÏßÂ··Ö³É1000¶Î
	{
		x += dis_x / 1000;//Ã¿´ÎÔö¼ÓÎ»ÒÆ/1000
		y += dis_y / 1000;
		if (x > 1366 || x < 0 || y > 680 || y < 0){
			is_on_way = true;
		}

		if (pow(x - 300, 2) + pow(y - 250, 2) < pow(77, 2)) //300,250ÊÇÕÏ°­Îï×ø±ê£¬°ë¾¶ÊÇ75£¬ÕâÀïÈ¡76¿¼ÂÇµ½Îó²î£¨76²»Ò»¶¨ºÃ£¬76ÊÇÕë¶Ô¹âÀâÊµÑéµÃ³öµÄÊýÖµ£©
			is_on_way = true;

		if (pow(x - 300, 2) + pow(y - 250, 2) < pow(83, 2)) //Èç¹û´ÓÖù×ÓÖ®¼ä´©¹ýÒª80
			is_on_half_way[0] = true;

		if (pow(x - 1066, 2) + pow(y - 430, 2) < pow(77, 2)) //ÁíÒ»¸öÕÏ°­Îï
			is_on_way = true;

		if (pow(x - 1066, 2) + pow(y - 430, 2) < pow(83, 2)) //Èç¹û´ÓÖù×ÓÖ®¼ä´©¹ýÒª80
			is_on_half_way[1] = true;
		if (pow(x - 300, 2) + pow(y - 250, 2) < pow(x_min - 300, 2) + pow(y_min - 250, 2) && pow(x - 300, 2) + pow(y - 250, 2) < pow(x_min - 1066, 2) + pow(y_min - 430, 2)){
			x_min = x;
			y_min = y;
		}
		else if (pow(x - 1066, 2) + pow(y - 430, 2) < pow(x_min - 300, 2) + pow(y_min - 250, 2) && pow(x - 1066, 2) + pow(y - 430, 2) < pow(x_min - 1066, 2) + pow(y_min - 430, 2)){
			x_min = x;
			y_min = y;
		}
	}

	if (pow(x_min - 300, 2) + pow(y_min - 250, 2) < pow(x_min - 1066, 2) + pow(y_min - 430, 2)){
		least_distance = sqrt(pow(x_min - 300, 2) + pow(y_min - 250, 2));
	}
	else{
		least_distance = sqrt(pow(x_min - 1066, 2) + pow(y_min - 430, 2));
	}

	return !(is_on_way || (is_on_half_way[0] && is_on_half_way[1])); //²»¹ÜÄÄ¸öÎªtrue¶¼²»·¢Éä£¬¶¼Îªfalse·¢Éä
}

void UFO_step(double &x, double &y, double &vx, double &vy, double &angle, const double run, const double eturn) //Õâ¸öÊÇ¸øUFOËãÒÆ¶¯Î»ÖÃµÄ ²ÎÊýÊÇ»ú¼×v,y,vx,vy,engine angle,ºÍ¼Ù¶¨µÄrunºÍeturn²ÎÊý£¨order.runºÍorder.eturn£©
{
	if (run != 0){
		vx += 0.2 * cos(angle / 180 * PI); //Èç¹ûÓÐ¼ÓËÙ¶È£¨run!=0)£¬ÄÇÃ´vx£¬vyÏàÓ¦µÄ±ä»¯
		vy += 0.2 * sin(angle / 180 * PI);


		if (pow(vx, 2) + pow(vy, 2) > pow(6, 2)){ //Èç¹û´óÓÚ×î¸ßËÙ¶ÈÔò°´×î¸ßËÙ¶È¼ÆËã
			double v_angle = atan2(vy, vx);
			vx = 6 * cos(v_angle);
			vy = 6 * sin(v_angle);
		}
	}

	x += vx; //ÏàÓ¦±äx£¬y
	y += vy;


	if (x < 46){//Èç¹û×²Ç½£¬ÓëÇ½´¹Ö±µÄËÙ¶È·ÖÁ¿Òª·´Ïò
		vx = -vx;
		x = 46;
	}
	else if (x > 1366 - 46){
		vx = -vx;
		x = 1366 - 46;
	}

	if (y < 46){
		vy = -vy;
		y = 46;
	}
	else if (y > 680 - 46){
		vy = -vy;
		y = 680 - 46;
	}

	if (pow(x - 1066, 2) + pow(y - 430, 2) < pow(75 + 46, 2)){ //Èç¹û×²Öù×Ó
		double ori_vx = vx;
		double ori_vy = vy;
		double crash_ang = atan2(y - 430, x - 1066);//×²»÷½Ç¶È
		double v_tan; //Óë×²»÷´¦ÇÐÏßÆ½ÐÐµÄËÙ¶È·ÖÁ¿£¨ËÙ¶È·½ÏòÊ¹ÓÃÓÒÊÖ¶¨Ôò£ºËÄÖ¸³¯ÏòÏÂÃæÌáµ½µÄv_orth·½Ïò£¬´óÄ´Ö¸³¯ÉÏ£¬´ÓÊÖ±³µ½ÊÖÐÄµÄ·½Ïò¾ÍÊÇËÙ¶È·½Ïò£©
		double v_orth; //Óë×²»÷´¦·¨ÏßÆ½ÐÐµÄËÙ¶È·ÖÁ¿
		if (x - 1066 > 0 && y - 430 > 0){//·ÖÀàÌÖÂÛ£¬×Ô¼º»­Í¼¿´...ÎÒÒªÊÇ¶¼Ð´³öÀ´µÃÀÛËÀ
			v_tan = vx*sin(crash_ang) - vy*cos(crash_ang);
			v_orth = vx*cos(crash_ang) + vy*sin(crash_ang);//ÆäÊµ×²»÷Ç°ÊÇ¸ÃÖµµÄ¸ºÖµ£¬µ«ÊÇ×²»÷ºó¸ÃÖµ·½Ïò±äÎª·´·½Ïò£¬ËùÒÔÖ±½ÓÐ´ÁË×²ºóµÄÖµ£¬ÏÂÍ¬
			vx = v_tan*sin(crash_ang) + v_orth*cos(crash_ang);
			vy = -v_tan*cos(crash_ang) + v_orth*sin(crash_ang);
		}
		else if (x - 1066 < 0 && y - 430 > 0){
			v_tan = vx*sin(crash_ang) + vy*cos(crash_ang);
			v_orth = -vx*cos(crash_ang) + vy*sin(crash_ang);
			vx = v_tan*sin(crash_ang) - v_orth*cos(crash_ang);
			vy = v_tan*cos(crash_ang) + v_orth*sin(crash_ang);
		}
		else if (x - 1066 > 0 && y - 430 < 0){
			v_tan = -vx*sin(crash_ang) - vy*cos(crash_ang);
			v_orth = vx*cos(crash_ang) - vy*sin(crash_ang);
			vx = v_tan*sin(crash_ang) - v_orth*cos(crash_ang);
			vy = -v_tan*cos(crash_ang) - v_orth*sin(crash_ang);
		}
		else if (x - 1066 < 0 && y - 430 < 0){
			v_tan = -vx*sin(crash_ang) + vy*cos(crash_ang);
			v_orth = vx*cos(crash_ang) + vy*sin(crash_ang);
			vx = -v_tan*sin(crash_ang) - v_orth*cos(crash_ang);
			vy = v_tan*cos(crash_ang) - v_orth*sin(crash_ang);
		}
		else if (x - 1066 == 0){//ÌØÊâµÄ×²»÷Î»ÖÃ
			vx = -vx;
		}
		else if (y - 430 == 0){
			vy = -vy;
		}
	}

	if (pow(x - 300, 2) + pow(y - 250, 2) < pow(75 + 46, 2)){//Í¬ÉÏ
		double ori_vx = vx;
		double ori_vy = vy;
		double crash_ang = atan2(y - 250, x - 300);
		double v_tan;
		double v_orth;

		if (x - 300 > 0 && y - 250 > 0){
			v_tan = vx*sin(crash_ang) - vy*cos(crash_ang);
			v_orth = vx*cos(crash_ang) + vy*sin(crash_ang);//ÆäÊµ×²»÷Ç°ÊÇ¸ÃÖµµÄ¸ºÖµ£¬µ«ÊÇ×²»÷ºó¸ÃÖµ·½Ïò±äÎª·´·½Ïò£¬ËùÒÔÖ±½ÓÐ´ÁË×²ºóµÄÖµ£¬ÏÂÍ¬
			vx = v_tan*sin(crash_ang) + v_orth*cos(crash_ang);
			vy = -v_tan*cos(crash_ang) + v_orth*sin(crash_ang);
		}
		else if (x - 300 < 0 && y - 250 > 0){
			v_tan = vx*sin(crash_ang) + vy*cos(crash_ang);
			v_orth = -vx*cos(crash_ang) + vy*sin(crash_ang);
			vx = v_tan*sin(crash_ang) - v_orth*cos(crash_ang);
			vy = v_tan*cos(crash_ang) + v_orth*sin(crash_ang);
		}
		else if (x - 300 > 0 && y - 250 < 0){
			v_tan = -vx*sin(crash_ang) - vy*cos(crash_ang);
			v_orth = vx*cos(crash_ang) - vy*sin(crash_ang);
			vx = v_tan*sin(crash_ang) - v_orth*cos(crash_ang);
			vy = -v_tan*cos(crash_ang) - v_orth*sin(crash_ang);
		}
		else if (x - 300 < 0 && y - 250 < 0){
			v_tan = -vx*sin(crash_ang) + vy*cos(crash_ang);
			v_orth = vx*cos(crash_ang) + vy*sin(crash_ang);
			vx = -v_tan*sin(crash_ang) - v_orth*cos(crash_ang);
			vy = v_tan*cos(crash_ang) - v_orth*sin(crash_ang);
		}
		else if (x - 300 == 0){
			vx = -vx;
		}
		else if (y - 250 == 0){
			vy = -vy;
		}
	}

	if (eturn > 0){
		angle += 4.5; //Ë³Ê±Õë×ª¶¯
	}
	else if (eturn < 0){
		angle -= 4.5; //ÄæÊ±Õë×ª¶¯
	}
	if (angle > 180){
		angle = angle - 360;
	}
	if (angle < -180){
		angle = angle + 360;
	}
}

void get_moves(const int a, int move[][2])
{
	const int MOVE_NUM = 6;
	const int RUN_NUM = 2;
	const int ETURN_NUM = 3;
	const int MOVE_TIMES = 5;

	for (int i = 0; i < MOVE_TIMES; i++){
		move[i][0] = (a % int(pow(MOVE_NUM, (i + 1)))) / int(pow(MOVE_NUM, i));
		move[i][1] = move[i][0] % ETURN_NUM - 1;
		move[i][0] = move[i][0] % RUN_NUM;
	}
}

void circle_move(const int NUM_HINDER, const RobotAI_BattlefieldInformation info, const int myID, double **UFO_route1, int move[])
{

	double circle_angle = atan2(-info.robotInformation[myID].circle.y + info.obstacle[NUM_HINDER].y, -info.robotInformation[myID].circle.x + info.obstacle[NUM_HINDER].x);
	if (NUM_HINDER == 0){
		circle_angle -= 0.285714;
	}
	else{
		circle_angle += 0.285714;
	}
	double aim_x = info.obstacle[NUM_HINDER].x + (75 + 46)*cos(circle_angle);
	double aim_y = info.obstacle[NUM_HINDER].y + (75 + 46)*sin(circle_angle);
	for (int i = 0; i < 7776; i++){
		double x, y, vx, vy, angle = 0;

		x = info.robotInformation[myID].circle.x;
		y = info.robotInformation[myID].circle.y;
		vx = info.robotInformation[myID].vx;
		vy = info.robotInformation[myID].vy;
		angle = info.robotInformation[myID].engineRotation;

		int steps[5][2] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };

		get_moves(i, steps);
		for (int j = 0; j < 5; j++){
			UFO_step(x, y, vx, vy, angle, steps[j][0], steps[j][1]);
		}
		UFO_route1[i][0] = sqrt(pow(x - aim_x, 2) + pow(y - aim_y, 2));
	}

	int best_route = 0;
	for (int i = 0; i < 7776; i++){
		if (UFO_route1[i][0] < UFO_route1[best_route][0]){
			best_route = i;
		}
	}
	move[0] = best_route % 6 % 2;
	move[1] = best_route % 6 % 3 - 1;
}


void center_circle_aim(const RobotAI_BattlefieldInformation info, const int myID, double aim_position[])
{
	const int TARGET_ID = 1 - myID;
	double weapon_angle = info.robotInformation[myID].weaponRotation;//ÏÖÔÚµÄÎäÆ÷½Ç¶È
	int weapon_rad = 69;
	double target_x = info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x - cos(weapon_angle / 180 * PI) * weapon_rad;//¶Ô·½×ø±ê
	double target_y = info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y - sin(weapon_angle / 180 * PI) * weapon_rad;

	double dis = sqrt(pow(target_x, 2) + pow(target_y, 2));//µÐÎÒ¾àÀë
	const double DIS_MAX = 1500;
	double radius = (DIS_MAX - dis + 100) / DIS_MAX * info.robotInformation[TARGET_ID].circle.r;
	if (radius > info.robotInformation[TARGET_ID].circle.r * 0.9){
		radius = info.robotInformation[TARGET_ID].circle.r * 0.9;
	}
	double best_dis = 0;
	for (int i = -180; i < 180 && radius > 0; i++){
		target_x = info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x - cos(weapon_angle / 180 * PI) * weapon_rad;//¶Ô·½×ø±ê
		target_y = info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y - sin(weapon_angle / 180 * PI) * weapon_rad;

		target_x += radius * cos(i / 180 * PI);
		target_y += radius * sin(i / 180 * PI);

		double min_dis = 0;
		effective_range(info.robotInformation[myID].circle.x + cos(weapon_angle / 180 * PI) * weapon_rad, info.robotInformation[myID].circle.y + sin(weapon_angle / 180 * PI) * weapon_rad, target_x, target_y, min_dis);
		if (min_dis > best_dis && min_dis > 75){
			best_dis = min_dis;
			aim_position[0] = target_x + info.robotInformation[myID].circle.x;
			aim_position[1] = target_y + info.robotInformation[myID].circle.y;
		}
	}
	if (best_dis < 75){
		aim_position[0] = 0;
		aim_position[1] = 0;
	}
}

void estimately_aim(const RobotAI_BattlefieldInformation info, const int myID, double estimate_ratio, double aim_position[]){
	const int TARGET_ID = 1 - myID;
	double weapon_angle = info.robotInformation[myID].weaponRotation;//ÏÖÔÚµÄÎäÆ÷½Ç¶È
	int weapon_rad = 69;
	double target_x = info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x - cos(weapon_angle / 180 * PI) * weapon_rad;//¶Ô·½×ø±ê
	double target_y = info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y - sin(weapon_angle / 180 * PI) * weapon_rad;

	double dis = sqrt(pow(target_x, 2) + pow(target_y, 2));//µÐÎÒ¾àÀë
	target_x = -info.robotInformation[myID].circle.x - cos(weapon_angle / 180 * PI) * weapon_rad;
	target_y = -info.robotInformation[myID].circle.y - sin(weapon_angle / 180 * PI) * weapon_rad;

	bool is_aimed = false;

	while (!is_aimed && estimate_ratio > 0.01){
		double x_after_move = info.robotInformation[TARGET_ID].circle.x;
		double y_after_move = info.robotInformation[TARGET_ID].circle.y;
		double vx = 0;
		double vy = 0;
		vx = info.robotInformation[TARGET_ID].vx;
		vy = info.robotInformation[TARGET_ID].vy;
		double best_x = x_after_move;
		double best_y = y_after_move;
		double best_dis = 30;
		bool is_ext = false;
		for (int i = 0; i < 200 && is_ext == false && x_after_move > 45 && x_after_move < 1366 - 45 && y_after_move > 45 && y_after_move < 680 - 45; i++){
			x_after_move += estimate_ratio * vx;
			y_after_move += estimate_ratio * vy;

			if (fabs(sqrt(pow(target_x + x_after_move, 2) + pow(target_y + y_after_move, 2)) - 14 * i) < 30 && sqrt(pow(target_x + x_after_move, 2) + fabs(pow(target_y + y_after_move, 2))) - 14 * i < best_dis){
				best_x = x_after_move;
				best_y = y_after_move;
				best_dis = fabs(sqrt(pow(target_x + x_after_move, 2) + pow(target_y + y_after_move, 2)) - 14 * i);
				is_aimed = true;
			}
			if (is_aimed == true && fabs(sqrt(pow(target_x + x_after_move, 2) + pow(target_y + y_after_move, 2)) - 14 * i) > 30){
				is_ext = true;
			}
		}
		estimate_ratio *= 0.9;
		aim_position[0] = best_x;
		aim_position[1] = best_y;
	}
	if (estimate_ratio < 0.01){
		aim_position[0] = info.robotInformation[TARGET_ID].circle.x;
		aim_position[1] = info.robotInformation[TARGET_ID].circle.y;
	}
}

void average_estimately_aim(const RobotAI_BattlefieldInformation info, const int myID, double estimate_ratio, double aim_position[]){
	const int TARGET_ID = 1 - myID;
	double weapon_angle = info.robotInformation[myID].weaponRotation;//ÏÖÔÚµÄÎäÆ÷½Ç¶È
	int weapon_rad = 69;
	double target_x = info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x - cos(weapon_angle / 180 * PI) * weapon_rad;//¶Ô·½×ø±ê
	double target_y = info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y - sin(weapon_angle / 180 * PI) * weapon_rad;

	double dis = sqrt(pow(target_x, 2) + pow(target_y, 2));//µÐÎÒ¾àÀë
	target_x = -info.robotInformation[myID].circle.x - cos(weapon_angle / 180 * PI) * weapon_rad;
	target_y = -info.robotInformation[myID].circle.y - sin(weapon_angle / 180 * PI) * weapon_rad;

	bool is_aimed = false;

	while (!is_aimed && estimate_ratio > 0.01){
		double x_after_move = info.robotInformation[TARGET_ID].circle.x;
		double y_after_move = info.robotInformation[TARGET_ID].circle.y;
		double vx = 0;
		double vy = 0;
		for (int i = 0; i < target_velocity_num; i++){
			vx += target_velocity[i][0];
			vy += target_velocity[i][1];
		}
		if (target_velocity_num == 0){
			vx = info.robotInformation[TARGET_ID].vx;
			vy = info.robotInformation[TARGET_ID].vy;
		}
		else{
			vx /= target_velocity_num;
			vy /= target_velocity_num;
		}
		double best_x = x_after_move;
		double best_y = y_after_move;
		double best_dis = 30;
		bool is_ext = false;
		for (int i = 0; i < 200 && is_ext == false && x_after_move > 45 && x_after_move < 1366 - 45 && y_after_move > 45 && y_after_move < 680 - 45; i++){
			x_after_move += estimate_ratio * vx;
			y_after_move += estimate_ratio * vy;

			if (fabs(sqrt(pow(target_x + x_after_move, 2) + pow(target_y + y_after_move, 2)) - 14 * i) < 30 && fabs(sqrt(pow(target_x + x_after_move, 2) + pow(target_y + y_after_move, 2)) - 14 * i) < best_dis){
				best_x = x_after_move;
				best_y = y_after_move;
				best_dis = fabs(sqrt(pow(target_x + x_after_move, 2) + pow(target_y + y_after_move, 2)) - 14 * i);
				is_aimed = true;
			}
			if (is_aimed == true && fabs(sqrt(pow(target_x + x_after_move, 2) + pow(target_y + y_after_move, 2)) - 14 * i) > 30){
				is_ext = true;
			}
		}
		estimate_ratio *= 0.9;
		aim_position[0] = best_x;
		aim_position[1] = best_y;
	}
	if (estimate_ratio < 0.01){
		aim_position[0] = info.robotInformation[TARGET_ID].circle.x;
		aim_position[1] = info.robotInformation[TARGET_ID].circle.y;
	}
}

void UFO_hide(const RobotAI_BattlefieldInformation info, const int myID, int move[], double **UFO_route1, double ***bullet_position1)
{
	if (change[2] == 0){
		const int TARGET_ID = 1 - myID;
		const int MOVE_NUM = 6;
		const int MOVE_TIMES = 5;
		const int MOVE_CIRCULE_TIMES = 5;
		const int ROUTE_LENGTH = 7776;
		const int BULLET_MAX = 100;
		double x, y, vx, vy;

		for (int i = 0; i < ROUTE_LENGTH; i++){
			for (int j = 0; j < 2; j++){
				UFO_route1[i][j] = 0;
			}
		}

		for (int i = 0; i < BULLET_MAX; i++){
			for (int j = 0; j < 500; j++){
				for (int k = 0; k < 2; k++){
					bullet_position1[i][j][k] = 0;
				}
			}
		}

		if ((info.robotInformation[myID].weaponTypeName == WT_Machinegun && ((info.robotInformation[myID].remainingAmmo < 7 && info.robotInformation[TARGET_ID].weaponTypeName != WT_ElectricSaw) || ((info.robotInformation[TARGET_ID].weaponTypeName == WT_ElectricSaw && sqrt(pow(info.robotInformation[myID].circle.x - info.robotInformation[TARGET_ID].circle.x, 2) + pow(info.robotInformation[myID].circle.y - info.robotInformation[TARGET_ID].circle.y, 2)) > pow(600, 2)) && info.robotInformation[myID].remainingAmmo < 1))) || (info.robotInformation[TARGET_ID].weaponTypeName == WT_Machinegun && info.robotInformation[myID].weaponTypeName == WT_Machinegun && info.robotInformation[myID].remainingAmmo < 15)){

			x = info.robotInformation[TARGET_ID].circle.x;
			y = info.robotInformation[TARGET_ID].circle.y;
			int arsenal_num = TARGET_ID;
			if (pow(info.robotInformation[myID].circle.x - info.arsenal[arsenal_num].circle.x, 2) + pow(info.robotInformation[myID].circle.y - info.arsenal[arsenal_num].circle.y, 2) > pow(info.robotInformation[TARGET_ID].circle.x - info.arsenal[arsenal_num].circle.x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.arsenal[arsenal_num].circle.y, 2)){
				arsenal_num = 1 - arsenal_num;
			}

			if (info.arsenal[arsenal_num].respawning_time > info.arsenal[1 - arsenal_num].respawning_time + 50){
				arsenal_num = 1 - arsenal_num;
			}

			for (int i = 0; i < ROUTE_LENGTH; i++){
				double dis = 0;

				double angle = 0;

				x = info.robotInformation[myID].circle.x;
				y = info.robotInformation[myID].circle.y;
				vx = info.robotInformation[myID].vx;
				vy = info.robotInformation[myID].vy;
				angle = info.robotInformation[myID].engineRotation;

				int steps[5][2] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };

				get_moves(i, steps);

				for (int j = 0; j < MOVE_TIMES; j++){
					for (int circle = 0; circle < MOVE_CIRCULE_TIMES; circle++){
						UFO_step(x, y, vx, vy, angle, steps[j][0], steps[j][1]);
					}
				}

				UFO_route1[i][0] = sqrt(pow(x - info.arsenal[arsenal_num].circle.x, 2) + pow(y - info.arsenal[arsenal_num].circle.y, 2));
			}

			int best_route = 0;

			for (int i = 0; i < ROUTE_LENGTH; i++){
				if (UFO_route1[i][0] < UFO_route1[best_route][0]){
					best_route = i;
				}
			}

			move[0] = best_route % MOVE_NUM % 2;
			move[1] = best_route % MOVE_NUM % 3 - 1;

			if (move[0] == 0){

				x = info.arsenal[arsenal_num].circle.x - info.robotInformation[myID].circle.x;
				y = info.arsenal[arsenal_num].circle.y - info.robotInformation[myID].circle.y;
				double angle = atan2(y, x) * 180 / PI;


				if (angle > info.robotInformation[myID].engineRotation){
					if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
						move[1] = 1;
					}
					else{
						move[1] = -1;
					}
				}
				else{
					if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
						move[1] = -1;
					}
					else{
						move[1] = 1;
					}
				}

				if (fabs(angle - info.robotInformation[myID].engineRotation) < 25){
					move[0] = 1;
				}
			}

		}

		else if (info.robotInformation[TARGET_ID].remainingAmmo < 1 || (info.robotInformation[TARGET_ID].weaponTypeName == WT_MissileLauncher && info.robotInformation[TARGET_ID].remainingAmmo < 3)){
			double angle = 0;
			double target_x = info.robotInformation[TARGET_ID].circle.x + info.robotInformation[TARGET_ID].vx * 6;
			double target_y = info.robotInformation[TARGET_ID].circle.y + info.robotInformation[TARGET_ID].vy * 6;
			for (int i = 0; i < 6; i++){
				x = info.robotInformation[myID].circle.x;
				y = info.robotInformation[myID].circle.y;
				vx = info.robotInformation[myID].vx;
				vy = info.robotInformation[myID].vy;
				angle = info.robotInformation[myID].engineRotation;
				UFO_step(x, y, vx, vy, angle, i % 2, i % 3 - 1);
				for (int j = 0; j < 5; j++){
					UFO_step(x, y, vx, vy, angle, 1, 0);
				}

				UFO_route1[i][0] = sqrt(pow(x - target_x, 2) + pow(y - target_y, 2));

				angle = -atan2(y - target_y, x - target_x) * 180 / PI;

				if (fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) < 180){
					angle = fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) - 6;
				}
				else{
					angle = fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) - 180 - 6;
				}

				if (angle < 0){
					angle = 0;
				}

				UFO_route1[i][1] = angle;
			}
			int best_route = 0;

			for (int i = 0; i < 6; i++){
				if (UFO_route1[i][1] - UFO_route1[best_route][1] > 1){
					best_route = i;
				}
				else if (UFO_route1[i][0] < UFO_route1[best_route][0]){
					best_route = i;
				}
			}

			move[0] = best_route % 2;
			move[1] = best_route % 3 - 1;

			if (move[0] == 0){

				x = info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x;
				y = info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y;
				angle = atan2(y, x) * 180 / PI;


				if (angle > info.robotInformation[myID].engineRotation){
					if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
						move[1] = 1;
					}
					else{
						move[1] = -1;
					}
				}
				else{
					if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
						move[1] = -1;
					}
					else{
						move[1] = 1;
					}
				}

				if (fabs(angle - info.robotInformation[myID].engineRotation) < 25){
					move[0] = 1;
				}
			}


		}
		else if (info.robotInformation[TARGET_ID].weaponTypeName == WT_Prism || info.robotInformation[TARGET_ID].weaponTypeName == WT_Tesla){
			double angle = 0;
			double target_x = info.robotInformation[TARGET_ID].circle.x + info.robotInformation[TARGET_ID].vx * 6;
			double target_y = info.robotInformation[TARGET_ID].circle.y + info.robotInformation[TARGET_ID].vy * 6;
			for (int i = 0; i < 6; i++){
				x = info.robotInformation[myID].circle.x;
				y = info.robotInformation[myID].circle.y;
				vx = info.robotInformation[myID].vx;
				vy = info.robotInformation[myID].vy;
				angle = info.robotInformation[myID].engineRotation;
				UFO_step(x, y, vx, vy, angle, i % 2, i % 3 - 1);
				for (int j = 0; j < 5; j++){
					UFO_step(x, y, vx, vy, angle, 1, 0);
				}

				UFO_route1[i][0] = sqrt(pow(x - target_x, 2) + pow(y - target_y, 2));

				angle = -atan2(y - target_y, x - target_x) * 180 / PI;

				if (fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) < 180){
					angle = fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) - 6;
				}
				else{
					angle = fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) - 180 - 6;
				}

				if (angle < 0){
					angle = 0;
				}

				UFO_route1[i][1] = angle;
			}
			int best_route = 0;

			for (int i = 0; i < 6; i++){
				if (UFO_route1[i][1] - UFO_route1[best_route][1] > 1){
					best_route = i;
				}
				else if (UFO_route1[i][0] < UFO_route1[best_route][0]){
					best_route = i;
				}
			}

			move[0] = best_route % 2;
			move[1] = best_route % 3 - 1;

			if (UFO_route1[best_route][0] < 150){
				move[0] = 1;
				move[1] = 1;
			}

			if (move[0] == 0){

				x = info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x;
				y = info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y;
				angle = atan2(y, x) * 180 / PI;


				if (angle > info.robotInformation[myID].engineRotation){
					if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
						move[1] = 1;
					}
					else{
						move[1] = -1;
					}
				}
				else{
					if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
						move[1] = -1;
					}
					else{
						move[1] = 1;
					}
				}

				if (fabs(angle - info.robotInformation[myID].engineRotation) < 25){
					move[0] = 1;
				}
			}


		}
		else if (info.robotInformation[TARGET_ID].weaponTypeName == WT_MissileLauncher){
			int bullet_num = 0;//¼ÆÊý±äÁ¿£¬¼ÆÊý¶Ô·½µÄ×Óµ¯ÊýÁ¿
			for (int i = 0; i < info.num_bullet && i < BULLET_MAX; i++){ //for every bullet on the battlefield

				if (info.bulletInformation[i].type != BT_MachinegunBall){ //Õâ¸ör¾ÝÑ§³¤Ëµ±íÊ¾ÊÇÄÄ¸ö»úÆ÷·¢Éä³öÀ´µÄ...²»ÖªµÀÔÚ¸üÐÂÖÐ¼ÓÈëÁËÃ»ÓÐ -999 = 0 999 = 1
					bullet_num += 1; //¼ÆÊý±äÁ¿+1
				}
			}

			if (bullet_num > 0 && info.robotInformation[myID].hp <= 15 * (2 + bullet_num + info.robotInformation[TARGET_ID].remainingAmmo)){
				for (int ammo = 0; ammo < info.num_bullet && ammo < BULLET_MAX; ammo++){
					if (info.bulletInformation[ammo].type != BT_MachinegunBall){ //Õâ¸ör¾ÝÑ§³¤Ëµ±íÊ¾ÊÇÄÄ¸ö»úÆ÷·¢Éä³öÀ´µÄ...²»ÖªµÀÔÚ¸üÐÂÖÐ¼ÓÈëÁËÃ»ÓÐ -999 = 0 999 = 1
						for (int i = 0; i < ROUTE_LENGTH; i++){
							double dis = 0;

							int is_explode[100];//Èç¹ûÒ»¸ö×Óµ¯»÷ÖÐÁË×Ô¼º£¬¾Í»áÔÚ½ÓÏÂÀ´µÄÊ±¼äÖÐ²»²ÎÓëÔËËã
							int is_exp_num = 0;//»÷ÖÐ×Ô¼ºµÄ×Óµ¯ÊýÁ¿

							double angle = 0;
							double ammo_angle = info.bulletInformation[ammo].rotation;
							double ammo_x = info.bulletInformation[ammo].circle.x;
							double ammo_y = info.bulletInformation[ammo].circle.y;
							bool is_ext = true;//±íÊ¾Õâ·¢×Óµ¯ÊÇ·ñ´æÔÚ£¨ÊÇ·ñ»¹Ã»ÓÐÔÚÒÔÇ°µÄÖ¡ÖÐ»÷ÖÐ×Ô¼º£©

							x = info.robotInformation[myID].circle.x;
							y = info.robotInformation[myID].circle.y;
							vx = info.robotInformation[myID].vx;
							vy = info.robotInformation[myID].vy;
							angle = info.robotInformation[myID].engineRotation;

							int steps[5][2] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };

							get_moves(i, steps);

							for (int j = 0; j < MOVE_TIMES; j++){
								for (int circle = 0; circle < MOVE_CIRCULE_TIMES; circle++){

									for (int l = 0; l < is_exp_num; l++){
										if (ammo == is_explode[l]){
											is_ext = false;//Èç¹ûÒÑ¾­ÔÚis_explode£¬ÄÇÃ´²»´æÔÚ
										}
									}

									UFO_step(x, y, vx, vy, angle, steps[j][0], steps[j][1]);

									double ammo_to_me_angle = 0;
									ammo_to_me_angle = atan2(y - ammo_y, x - ammo_x) * 180 / PI;
									ammo_x += 4 * cos(ammo_angle / 180 * PI);
									ammo_y += 4 * sin(ammo_angle / 180 * PI);
									if (ammo_angle > ammo_to_me_angle){
										if (fabs(ammo_to_me_angle - ammo_angle) < 180){
											ammo_angle -= 1.5;
										}
										else{
											ammo_angle += 1.5;
										}
									}
									else{
										if (fabs(ammo_to_me_angle - ammo_angle) < 180){
											ammo_angle += 1.5;
										}
										else{
											ammo_angle -= 1.5;
										}
									}
									if (ammo_angle < -180){
										ammo_angle += 360;
									}
									else if (ammo_angle > 180){
										ammo_angle -= 360;
									}

									if ((ammo_x < 0 || ammo_x > 1366 || ammo_y < 0 || ammo_y > 680 || pow(ammo_x - 1066, 2) + pow(ammo_y - 430, 2) < pow(75, 2) || pow(ammo_x - 300, 2) + pow(ammo_y - 250, 2) < pow(75, 2)) && is_ext){
										is_explode[is_exp_num] = ammo;//°Ñ¸Ã×Óµ¯µÄ±êºÅ¼ÆÈëis_explode
										is_exp_num += 1;//is_explode¼ÆÊý+1
										UFO_route1[i][0] += 1;//is_explode¼ÆÊý+1
										is_ext = false;
									}



									if (sqrt(pow(ammo_x - x, 2) + pow(ammo_y - y, 2)) < 46 && is_ext){
										UFO_route1[i][1] += 1;//¼ÆÊý±»»÷ÖÐ´ÎÊý+1
										is_explode[is_exp_num] = ammo;//°Ñ¸Ã×Óµ¯µÄ±êºÅ¼ÆÈëis_explode
										is_exp_num += 1;//is_explode¼ÆÊý+1
										is_ext = false;
									}
								}
							}
							is_exp_num = 0;
						}

					}
				}


				int best_route = 0;
				for (int i = 0; i < ROUTE_LENGTH; i++){
					if (UFO_route1[i][1] - UFO_route1[best_route][1] < -0.1){
						best_route = i;
					}
					else if (UFO_route1[i][0] > UFO_route1[best_route][0] &&
						fabs(UFO_route1[i][1] - UFO_route1[best_route][1]) < 0.1){
						best_route = i;
					}
				}

				move[0] = best_route % MOVE_NUM % 2;
				move[1] = best_route % MOVE_NUM % 3 - 1;
				change[0] = move[0];
				change[1] = move[1];
				change[2] = 4;
			}
			else{
				double angle = 0;
				double target_x = info.robotInformation[TARGET_ID].circle.x + info.robotInformation[TARGET_ID].vx * 6;
				double target_y = info.robotInformation[TARGET_ID].circle.y + info.robotInformation[TARGET_ID].vy * 6;
				for (int i = 0; i < 6; i++){
					x = info.robotInformation[myID].circle.x;
					y = info.robotInformation[myID].circle.y;
					vx = info.robotInformation[myID].vx;
					vy = info.robotInformation[myID].vy;
					angle = info.robotInformation[myID].engineRotation;
					UFO_step(x, y, vx, vy, angle, i % 2, i % 3 - 1);
					for (int j = 0; j < 5; j++){
						UFO_step(x, y, vx, vy, angle, 1, 0);
					}

					UFO_route1[i][0] = sqrt(pow(x - target_x, 2) + pow(y - target_y, 2));

					angle = -atan2(y - target_y, x - target_x) * 180 / PI;

					if (fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) < 180){
						angle = fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) - 6;
					}
					else{
						angle = fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) - 180 - 6;
					}

					if (angle < 0){
						angle = 0;
					}

					UFO_route1[i][1] = angle;
				}
				int best_route = 0;

				for (int i = 0; i < 6; i++){
					if (UFO_route1[i][1] - UFO_route1[best_route][1] > 1){
						best_route = i;
					}
					else if (UFO_route1[i][0] < UFO_route1[best_route][0]){
						best_route = i;
					}
				}

				move[0] = best_route % 2;
				move[1] = best_route % 3 - 1;

				if (move[0] == 0){

					x = info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x;
					y = info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y;
					angle = atan2(y, x) * 180 / PI;


					if (angle > info.robotInformation[myID].engineRotation){
						if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
							move[1] = 1;
						}
						else{
							move[1] = -1;
						}
					}
					else{
						if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
							move[1] = -1;
						}
						else{
							move[1] = 1;
						}
					}

					if (fabs(angle - info.robotInformation[myID].engineRotation) < 25){
						move[0] = 1;
					}
				}
			}
		}
		else if (info.robotInformation[TARGET_ID].weaponTypeName == WT_RPG){

			int bullet_num = 0;//¼ÆÊý±äÁ¿£¬¼ÆÊý¶Ô·½µÄ×Óµ¯ÊýÁ¿
			for (int i = 0; i < info.num_bullet && i < BULLET_MAX; i++){ //for every bullet on the battlefield

				if (info.bulletInformation[i].type != BT_MachinegunBall){ //Õâ¸ör¾ÝÑ§³¤Ëµ±íÊ¾ÊÇÄÄ¸ö»úÆ÷·¢Éä³öÀ´µÄ...²»ÖªµÀÔÚ¸üÐÂÖÐ¼ÓÈëÁËÃ»ÓÐ -999 = 0 999 = 1
					x = info.bulletInformation[i].circle.x; //Õâ×Ü²»ÓÃ×¢ÊÍÁË°É...
					y = info.bulletInformation[i].circle.y;

					vx = info.bulletInformation[i].vx;
					vy = info.bulletInformation[i].vy;

					bool is_crashed = false;

					for (int j = 0; j < 100; j++){
						vx = vx*1.05;
						vy = vy*1.05;
						x += vx;
						y += vy;
						if (x < 0 || x > 1366 || y < 0 || y > 680 || pow(x - 1066, 2) + pow(y - 430, 2) < pow(75, 2) || pow(x - 300, 2) + pow(y - 250, 2) < pow(75, 2)){
							bullet_position1[bullet_num][j][0] = x;
							bullet_position1[bullet_num][j][1] = y;
						}
						else{
							bullet_position1[bullet_num][j][0] = x;
							bullet_position1[bullet_num][j][1] = y;
						}
					}

					bullet_num += 1; //¼ÆÊý±äÁ¿+1
				}
			}
			if (bullet_num > 0){
				for (int i = 0; bullet_num != 0 && i < ROUTE_LENGTH; i++){
					double dis = 0;

					int is_explode[100];//Èç¹ûÒ»¸ö×Óµ¯»÷ÖÐÁË×Ô¼º£¬¾Í»áÔÚ½ÓÏÂÀ´µÄÊ±¼äÖÐ²»²ÎÓëÔËËã
					int is_exp_num = 0;//»÷ÖÐ×Ô¼ºµÄ×Óµ¯ÊýÁ¿

					double angle = 0;

					x = info.robotInformation[myID].circle.x;
					y = info.robotInformation[myID].circle.y;
					vx = info.robotInformation[myID].vx;
					vy = info.robotInformation[myID].vy;
					angle = info.robotInformation[myID].engineRotation;

					int steps[5][2] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };

					get_moves(i, steps);
					for (int j = 0; j < MOVE_TIMES; j++){
						int circle_begin = 0;
						if (j == 0){
							circle_begin = 1;
						}
						for (int circle = circle_begin; circle < MOVE_CIRCULE_TIMES; circle++){
							UFO_step(x, y, vx, vy, angle, steps[j][0], steps[j][1]);
							for (int k = 0; k < bullet_num; k++){ //ilÊÇÖ®Ç°ÓÃµÄÊý¶Ô·½×Óµ¯µÄ¼ÆÊý±äÁ¿							
								bool is_ext = true;//±íÊ¾Õâ·¢×Óµ¯ÊÇ·ñ´æÔÚ£¨ÊÇ·ñ»¹Ã»ÓÐÔÚÒÔÇ°µÄÖ¡ÖÐ»÷ÖÐ×Ô¼º£©
								for (int l = 0; l < is_exp_num; l++){
									if (k == is_explode[l]){
										is_ext = false;//Èç¹ûÒÑ¾­ÔÚis_explode£¬ÄÇÃ´²»´æÔÚ
									}
								}


								if (bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES][0] == 0 && bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES][1] == 0 && is_ext){
									is_ext = false;
									is_explode[is_exp_num] = k;//°Ñ¸Ã×Óµ¯µÄ±êºÅ¼ÆÈëis_explode
									is_exp_num += 1;//is_explode¼ÆÊý+1

								}

								for (int l = 0; l < 100 && is_ext; l++){
									double x_now = (bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES][0] - bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES - 1][0]) / 100 * l + bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES - 1][0];
									double y_now = (bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES][1] - bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES - 1][1]) / 100 * l + bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES - 1][1];
									if (((x < 0 || x > 1366 || y < 0 || y > 680 || pow(x - 1066, 2) + pow(y - 430, 2) < pow(75, 2) || pow(x - 300, 2) + pow(y - 250, 2) < pow(75, 2)) && sqrt(pow(x_now - x, 2) + pow(y_now - y, 2)) < 103) || sqrt(pow(bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES][0] - x, 2) + pow(bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES][1] - y, 2)) < 46){
										UFO_route1[i][1] += 1;//¼ÆÊý±»»÷ÖÐ´ÎÊý+1
										is_explode[is_exp_num] = k;//°Ñ¸Ã×Óµ¯µÄ±êºÅ¼ÆÈëis_explode
										is_exp_num += 1;//is_explode¼ÆÊý+1
										is_ext = false;
									}
								}
							}
							circle_begin = 0;
						}
					}
					UFO_route1[i][0] = sqrt(pow(x - bullet_position1[0][0][0], 2) + pow(y - bullet_position1[0][0][1], 2));
					is_exp_num = 0;
				}

				int best_route = 0;
				for (int i = 0; i < ROUTE_LENGTH; i++){
					if (UFO_route1[i][1] - UFO_route1[best_route][1] < -0.1){
						best_route = i;
					}
					else if (UFO_route1[i][0] > UFO_route1[best_route][0] &&
						fabs(UFO_route1[i][1] - UFO_route1[best_route][1]) < 0.1){
						best_route = i;
					}
				}

				move[0] = best_route % MOVE_NUM % 2;
				move[1] = best_route % MOVE_NUM % 3 - 1;
				change[0] = move[0];
				change[1] = move[1];
				change[2] = 4;
			}
			else{
				double angle = 0;
				double target_x = info.robotInformation[TARGET_ID].circle.x + info.robotInformation[TARGET_ID].vx * 6;
				double target_y = info.robotInformation[TARGET_ID].circle.y + info.robotInformation[TARGET_ID].vy * 6;
				for (int i = 0; i < 6; i++){
					x = info.robotInformation[myID].circle.x;
					y = info.robotInformation[myID].circle.y;
					vx = info.robotInformation[myID].vx;
					vy = info.robotInformation[myID].vy;
					angle = info.robotInformation[myID].engineRotation;
					UFO_step(x, y, vx, vy, angle, i % 2, i % 3 - 1);
					for (int j = 0; j < 5; j++){
						UFO_step(x, y, vx, vy, angle, 1, 0);
					}

					UFO_route1[i][0] = sqrt(pow(x - target_x, 2) + pow(y - target_y, 2));

					angle = -atan2(y - target_y, x - target_x) * 180 / PI;

					if (fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) < 180){
						angle = fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) - 6;
					}
					else{
						angle = fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) - 180 - 6;
					}

					if (angle < 0){
						angle = 0;
					}

					UFO_route1[i][1] = angle;
				}
				int best_route = 0;

				for (int i = 0; i < 6; i++){
					if (UFO_route1[i][1] - UFO_route1[best_route][1] > 1){
						best_route = i;
					}
					else if (UFO_route1[i][0] < UFO_route1[best_route][0]){
						best_route = i;
					}
				}

				move[0] = best_route % 2;
				move[1] = best_route % 3 - 1;

				if (move[0] == 0){

					x = info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x;
					y = info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y;
					angle = atan2(y, x) * 180 / PI;


					if (angle > info.robotInformation[myID].engineRotation){
						if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
							move[1] = 1;
						}
						else{
							move[1] = -1;
						}
					}
					else{
						if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
							move[1] = -1;
						}
						else{
							move[1] = 1;
						}
					}

					if (fabs(angle - info.robotInformation[myID].engineRotation) < 25){
						move[0] = 1;
					}
				}
			}
		}
		else if (info.robotInformation[TARGET_ID].weaponTypeName == WT_PlasmaTorch){
			double angle = 0;
			double target_x = info.robotInformation[TARGET_ID].circle.x + info.robotInformation[TARGET_ID].vx * 6;
			double target_y = info.robotInformation[TARGET_ID].circle.y + info.robotInformation[TARGET_ID].vy * 6;
			for (int i = 0; i < 6; i++){
				x = info.robotInformation[myID].circle.x;
				y = info.robotInformation[myID].circle.y;
				vx = info.robotInformation[myID].vx;
				vy = info.robotInformation[myID].vy;
				angle = info.robotInformation[myID].engineRotation;
				UFO_step(x, y, vx, vy, angle, i % 2, i % 3 - 1);
				for (int j = 0; j < 5; j++){
					UFO_step(x, y, vx, vy, angle, 1, 0);
				}

				UFO_route1[i][0] = sqrt(pow(x - target_x, 2) + pow(y - target_y, 2));

				angle = -atan2(y - target_y, x - target_x) * 180 / PI;

				if (fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) < 180){
					angle = fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) - 6;
				}
				else{
					angle = fabs(angle - info.robotInformation[TARGET_ID].weaponRotation) - 180 - 6;
				}

				if (angle < 0){
					angle = 0;
				}

				UFO_route1[i][1] = angle;
			}
			int best_route = 0;

			for (int i = 0; i < 6; i++){
				if (UFO_route1[i][1] - UFO_route1[best_route][1] > 1){
					best_route = i;
				}
				else if (UFO_route1[i][0] < UFO_route1[best_route][0]){
					best_route = i;
				}
			}

			move[0] = best_route % 2;
			move[1] = best_route % 3 - 1;

			if (move[0] == 0){

				x = info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x;
				y = info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y;
				angle = atan2(y, x) * 180 / PI;


				if (angle > info.robotInformation[myID].engineRotation){
					if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
						move[1] = 1;
					}
					else{
						move[1] = -1;
					}
				}
				else{
					if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
						move[1] = -1;
					}
					else{
						move[1] = 1;
					}
				}

				if (fabs(angle - info.robotInformation[myID].engineRotation) < 25){
					move[0] = 1;
				}
			}
		}

		else if ((info.robotInformation[TARGET_ID].weaponTypeName == WT_ElectricSaw || info.robotInformation[TARGET_ID].weaponTypeName == WT_MineLayer || info.robotInformation[TARGET_ID].weaponTypeName == WT_GrenadeThrower) && sqrt(pow(info.robotInformation[myID].circle.x - info.robotInformation[TARGET_ID].circle.x, 2) + pow(info.robotInformation[myID].circle.y - info.robotInformation[TARGET_ID].circle.y, 2)) < pow(600, 2)){
			double angle = 0;
			for (int i = 0; i < 6; i++){
				x = info.robotInformation[myID].circle.x;
				y = info.robotInformation[myID].circle.y;
				vx = info.robotInformation[myID].vx;
				vy = info.robotInformation[myID].vy;
				angle = info.robotInformation[myID].engineRotation;
				UFO_step(x, y, vx, vy, angle, i % 2, i % 3 - 1);
				for (int j = 0; j < 5; j++){
					UFO_step(x, y, vx, vy, angle, 1, 0);
				}

				UFO_route1[i][0] = sqrt(pow(x - info.robotInformation[TARGET_ID].circle.x - 7 * info.robotInformation[TARGET_ID].vx, 2) + pow(y - info.robotInformation[TARGET_ID].circle.y - 7 * info.robotInformation[TARGET_ID].vy, 2));
			}
			int best_route = 0;

			for (int i = 0; i < 6; i++){
				if (UFO_route1[i][0] > UFO_route1[best_route][0] - 0.1){
					best_route = i;
				}
			}

			move[0] = best_route % 2;
			move[1] = best_route % 3 - 1;
			if (move[0] == 0){
				if (info.robotInformation[myID].circle.x > 1066 && info.robotInformation[myID].circle.y > 430){
					x = fabs(info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x);
					y = fabs(info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y);
					if (x > y){
						angle = -90;
					}
					else{
						angle = 180;
					}
				}
				else if (info.robotInformation[myID].circle.x < 300 && info.robotInformation[myID].circle.y > 430){
					x = fabs(info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x);
					y = fabs(info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y);
					if (x > y){
						angle = -90;
					}
					else{
						angle = 0;
					}
				}
				else if (info.robotInformation[myID].circle.x > 1066 && info.robotInformation[myID].circle.y < 300){
					x = fabs(info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x);
					y = fabs(info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y);
					if (x > y){
						angle = 90;
					}
					else{
						angle = 180;
					}
				}
				else if (info.robotInformation[myID].circle.x < 250 && info.robotInformation[myID].circle.y < 300){
					x = fabs(info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x);
					y = fabs(info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y);
					if (x > y){
						angle = 90;
					}
					else{
						angle = 0;
					}
				}
				else{
					x = -info.robotInformation[TARGET_ID].circle.x + info.robotInformation[myID].circle.x;
					y = -info.robotInformation[TARGET_ID].circle.y + info.robotInformation[myID].circle.y;
					angle = atan2(y, x) * 180 / PI;
				}

				if (angle > info.robotInformation[myID].engineRotation){
					if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
						move[1] = 1;
					}
					else{
						move[1] = -1;
					}
				}
				else{
					if (fabs(angle - info.robotInformation[myID].engineRotation) < 180){
						move[1] = -1;
					}
					else{
						move[1] = 1;
					}
				}

				if (fabs(angle - info.robotInformation[myID].engineRotation) < 25){
					move[0] = 1;
				}
			}

		}
		else if (info.robotInformation[TARGET_ID].weaponTypeName == WT_Cannon || info.robotInformation[TARGET_ID].weaponTypeName == WT_Apollo || info.robotInformation[TARGET_ID].weaponTypeName == WT_Shotgun || info.robotInformation[TARGET_ID].weaponTypeName == WT_Machinegun){
			int bullet_num = 0;//¼ÆÊý±äÁ¿£¬¼ÆÊý¶Ô·½µÄ×Óµ¯ÊýÁ¿
			int eff_bullet_num = 0;
			for (int i = 0; i < info.num_bullet && i < BULLET_MAX; i++){ //for every bullet on the battlefield

				if (true || info.bulletInformation[i].type != BT_MachinegunBall){ //Õâ¸ör¾ÝÑ§³¤Ëµ±íÊ¾ÊÇÄÄ¸ö»úÆ÷·¢Éä³öÀ´µÄ...²»ÖªµÀÔÚ¸üÐÂÖÐ¼ÓÈëÁËÃ»ÓÐ -999 = 0 999 = 1
					x = info.bulletInformation[i].circle.x; //Õâ×Ü²»ÓÃ×¢ÊÍÁË°É...
					y = info.bulletInformation[i].circle.y;

					vx = info.bulletInformation[i].vx;
					vy = info.bulletInformation[i].vy;

					bool is_crashed = false;

					for (int j = 0; !is_crashed && j < 50; j++){
						x += vx;
						y += vy;
						if (x < 0 || x > 1366 || y < 0 || y > 680 || pow(x - 1066, 2) + pow(y - 430, 2) < pow(75, 2) || pow(x - 300, 2) + pow(y - 250, 2) < pow(75, 2)){
							is_crashed = true;
						}
						else{
							bullet_position1[bullet_num][j][0] = x;
							bullet_position1[bullet_num][j][1] = y;
						}
					}

					bullet_num += 1; //¼ÆÊý±äÁ¿+1
					if (sqrt(pow(bullet_position1[i][0][0] - info.robotInformation[myID].circle.x, 2) + pow(bullet_position1[i][0][1] - info.robotInformation[myID].circle.y, 2)) >	sqrt(pow(bullet_position1[i][1][0] - info.robotInformation[myID].circle.x, 2) + pow(bullet_position1[i][1][1] - info.robotInformation[myID].circle.y, 2))){
						eff_bullet_num += 1;
					}
				}
			}
			for (int i = 0; bullet_num != 0 && i < ROUTE_LENGTH; i++){
				double dis = 0;

				int is_explode[100];//Èç¹ûÒ»¸ö×Óµ¯»÷ÖÐÁË×Ô¼º£¬¾Í»áÔÚ½ÓÏÂÀ´µÄÊ±¼äÖÐ²»²ÎÓëÔËËã
				int is_exp_num = 0;//»÷ÖÐ×Ô¼ºµÄ×Óµ¯ÊýÁ¿

				double angle = 0;

				x = info.robotInformation[myID].circle.x;
				y = info.robotInformation[myID].circle.y;
				vx = info.robotInformation[myID].vx;
				vy = info.robotInformation[myID].vy;
				angle = info.robotInformation[myID].engineRotation;

				int steps[5][2] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };

				get_moves(i, steps);
				for (int j = 0; j < MOVE_TIMES; j++){

					for (int circle = 0; circle < MOVE_CIRCULE_TIMES; circle++){
						UFO_step(x, y, vx, vy, angle, steps[j][0], steps[j][1]);

						for (int k = 0; k < bullet_num; k++){ //ilÊÇÖ®Ç°ÓÃµÄÊý¶Ô·½×Óµ¯µÄ¼ÆÊý±äÁ¿							
							bool is_ext = true;//±íÊ¾Õâ·¢×Óµ¯ÊÇ·ñ´æÔÚ£¨ÊÇ·ñ»¹Ã»ÓÐÔÚÒÔÇ°µÄÖ¡ÖÐ»÷ÖÐ×Ô¼º£©
							for (int l = 0; l < is_exp_num; l++){
								if (k == is_explode[l]){
									is_ext = false;//Èç¹ûÒÑ¾­ÔÚis_explode£¬ÄÇÃ´²»´æÔÚ
								}
							}


							if (bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES][0] == 0 && bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES][1] == 0 && is_ext){
								is_ext = false;
								is_explode[is_exp_num] = k;//°Ñ¸Ã×Óµ¯µÄ±êºÅ¼ÆÈëis_explode
								is_exp_num += 1;//is_explode¼ÆÊý+1
							}

							if (sqrt(pow(bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES][0] - x, 2) + pow(bullet_position1[k][circle + j*MOVE_CIRCULE_TIMES][1] - y, 2)) < 46 && is_ext){ //Èç¹û»÷ÖÐ£¬°ë¾¶ÊÇ46£¬Ê¹ÓÃÒ»¸öÉÔÎ¢´óÒ»µãµÄ48ÒÔ·ÀÎó²î
								UFO_route1[i][1] += 1;//¼ÆÊý±»»÷ÖÐ´ÎÊý+1
								is_explode[is_exp_num] = k;//°Ñ¸Ã×Óµ¯µÄ±êºÅ¼ÆÈëis_explode
								is_exp_num += 1;//is_explode¼ÆÊý+1
							}

						}
					}
				}
				for (int j = 0; j < bullet_num; j++){ //ilÊÇÖ®Ç°ÓÃµÄÊý¶Ô·½×Óµ¯µÄ¼ÆÊý±äÁ¿
					bool is_ext = true;//±íÊ¾Õâ·¢×Óµ¯ÊÇ·ñ´æÔÚ£¨ÊÇ·ñ»¹Ã»ÓÐÔÚÒÔÇ°µÄÖ¡ÖÐ»÷ÖÐ×Ô¼º£©
					for (int k = 0; k < is_exp_num; k++){
						if (j == is_explode[k]){
							is_ext = false;//Èç¹ûÒÑ¾­is_explode£¬ÄÇÃ´²»´æÔÚ
						}
					}
					if (is_ext &&
						(sqrt(pow(bullet_position1[j][0][0] - info.robotInformation[myID].circle.x, 2) + pow(bullet_position1[j][0][1] - info.robotInformation[myID].circle.y, 2)) >
						sqrt(pow(bullet_position1[j][1][0] - info.robotInformation[myID].circle.x, 2) + pow(bullet_position1[j][1][1] - info.robotInformation[myID].circle.y, 2)) ||
						sqrt(pow(bullet_position1[j][0][0] - info.robotInformation[myID].circle.x, 2) + pow(bullet_position1[j][0][1] - info.robotInformation[myID].circle.y, 2)) < 50)){

						double dis_to_me = 0;//×Óµ¯ºÍ»ú¼×µÄ¾àÀë^2

						for (int k = 0; k < 50 && (bullet_position1[j][k][0] != 0 || bullet_position1[j][k][1] != 0); k++){
							if (sqrt(pow(bullet_position1[j][k][0] - x, 2) + pow(bullet_position1[j][k][1] - y, 2)) < dis_to_me){
								dis_to_me = sqrt(pow(bullet_position1[j][k][0] - x, 2) + pow(bullet_position1[j][k][1] - y, 2));
							}
						}
						dis += 1000 / fabs(dis_to_me);//ÓÃ1/(×Óµ¯Óë»ú¼×µÄ¾àÀë^2-»ú¼×°ë¾¶Æ½·½)£¬¶ÔËùÓÐ×Óµ¯¶¼ÇóÕâ¸öÖµÈ»ºó¼ÓÆðÀ´¿ÉÒÔµÃµ½Ò»¸öÀë³¡ÉÏËùÓÐ×Óµ¯¾àÀëµÄÊýÖµ£¨Ô½½üµÄ×Óµ¯¶ÔÊýÖµ¹±Ï×Ô½´ó£©

					}
					UFO_route1[i][0] = dis;
				}
			}

			int best_route = 0;
			for (int i = 0; i < ROUTE_LENGTH; i++){
				if (UFO_route1[i][1] - UFO_route1[best_route][1] < -0.1){
					best_route = i;
				}
				else if (UFO_route1[i][0] > UFO_route1[best_route][0] &&
					fabs(UFO_route1[i][1] - UFO_route1[best_route][1]) < 0.1){
					best_route = i;
				}
			}

			move[0] = best_route % MOVE_NUM % 2;
			move[1] = best_route % MOVE_NUM % 3 - 1;
			change[0] = move[0];
			change[1] = move[1];
			if (eff_bullet_num > 5){
				change[2] = 0;
			}
			else{
				change[2] = 4;
			}


		}
		else{
			move[0] = -1;
		}
	}
	else{
		move[0] = change[0];
		move[1] = change[1];
		change[2] -= 1;
	}
}

void funny_useless_quotes()
{
	switch (rand() % 40){
	case 0:
		std::cout << "Disc in flight. --- from Useless" << endl;
		break;
	case 1:
		std::cout << "Spinning away! --- from Useless" << endl;
		break;
	case 2:
		std::cout << "They are in our way! --- from Useless" << endl;
		break;
	case 3:
		std::cout << "Smooth ride! --- from Useless" << endl;
		break;
	case 4:
		std::cout << "Set to destory. --- from Useless" << endl;
		break;
	case 5:
		std::cout << "Their systems are vulnerable. --- from Useless" << endl;
		break;
	case 6:
		std::cout << "Their taxes are due --- from Useless" << endl;
		break;
	case 7:
		std::cout << "Levitation units online --- from Useless" << endl;
		break;
	case 8:
		std::cout << "We need no compass --- from Useless" << endl;
		break;
	case 9:
		std::cout << "Let the bullets fly! --- from Useless" << endl;
		break;
	case 10:
		std::cout << "I love the smell of gunpowder! --- from Useless" << endl;
		break;
	case 11:
		std::cout << "Gun Barrels cooled and ready --- from Useless" << endl;
		break;
	case 12:
		std::cout << "Guns ready for full auto --- from Useless" << endl;
		break;
	case 13:
		std::cout << "Scanning horizon --- from Useless" << endl;
		break;
	case 14:
		std::cout << "Faster and faster! --- from Useless" << endl;
		break;
	case 15:
		std::cout << "Let's take the guns for a spin --- from Useless" << endl;
		break;
	case 16:
		std::cout << "Mow them down! --- from Useless" << endl;
		break;
	case 17:
		std::cout << "How about a shell shower! --- from Useless" << endl;
		break;
	case 18:
		std::cout << "Guns perfer auto --- from Useless" << endl;
		break;
	case 19:
		std::cout << "Take me to frontlines --- from Useless" << endl;
		break;
	case 20:
		std::cout << "They will not escape! --- from Useless" << endl;
		break;
	case 21:
		std::cout << "We have'm on radar --- from Useless" << endl;
		break;
	case 22:
		std::cout << "These will be your last moments. --- from Useless" << endl;
		break;
	case 23:
		std::cout << "Gravity? What's that? --- from Useless" << endl;
		break;
	case 24:
		std::cout << "Shifting thrusters --- from Useless" << endl;
		break;
	case 25:
		std::cout << "Boosters engaged --- from Useless" << endl;
		break;
	case 26:
		std::cout << "Tracking target --- from Useless" << endl;
		break;
	case 27:
		std::cout << "Fuelmix optimized --- from Useless" << endl;
		break;
	case 28:
		std::cout << "I cast a deadly shadow. --- from Useless" << endl;
		break;
	case 29:
		std::cout << "Gunner in position --- from Useless" << endl;
		break;
	case 30:
		std::cout << "Learning the path --- from Useless" << endl;
		break;
	case 31:
		std::cout << "They're shooting me --- from Useless" << endl;
		break;
	case 32:
		std::cout << "I have an important mission. --- from Useless" << endl;
		break;
	case 33:
		std::cout << "I'm not swimming out there! Get me an armoured transport from shipyard. --- from Useless" << endl;
		break;
	case 34:
		std::cout << "The end is near --- from Useless" << endl;
		break;
	case 35:
		std::cout << "Spread the Doom --- from Useless" << endl;
		break;
	case 36:
		std::cout << "I go freely --- from Useless" << endl;
		break;
	case 37:
		std::cout << "Target sighted --- from Useless" << endl;
		break;
	case 38:
		std::cout << "Changing position --- from Useless" << endl;
		break;
	case 39:
		std::cout << "Encounting enemy --- from Useless" << endl;
		break;
	}
}

void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{

	const int TARGET_ID = 1 - myID;//×Ü¹²Á½¸ö£¬²»ÊÇÎÒµÄid¾ÍÊÇ¶Ô·½µÄid
	int shoot_dis = 1500;
	int weapon_rad = 69;
	frames += 1;//Ò»¸öºÜÎÞÁÄµÄ¼ÆÊý±äÁ¿£¬¼ÆÊýÊÇµÚ¼¸Ö¡
	if (frames % 10 == 0){
		funny_useless_quotes();
	}
	if (frames > 0){
		if (target_velocity_num < 10){
			target_velocity[target_velocity_num][0] = info.robotInformation[TARGET_ID].vx;
			target_velocity[target_velocity_num][1] = info.robotInformation[TARGET_ID].vy;
			target_velocity_num += 1;
		}
		else if (target_velocity_num > 9){
			for (int i = 1; i < 10; i++){
				target_velocity[i - 1][0] = target_velocity[i][0];
				target_velocity[i - 1][1] = target_velocity[i][1];
			}
			target_velocity[9][0] = info.robotInformation[TARGET_ID].vx;
			target_velocity[9][1] = info.robotInformation[TARGET_ID].vy;
		}
	}


	double aim_position[2];

	if (info.robotInformation[TARGET_ID].remainingAmmo <= 1){
		estimately_aim(info, myID, 0.9, aim_position);
	}
	else{
		if (TARGET_ENGINE != 1){
			average_estimately_aim(info, myID, 0.8 * sqrt(pow(info.robotInformation[myID].circle.x - info.robotInformation[TARGET_ID].circle.x, 2) + pow(info.robotInformation[myID].circle.y - info.robotInformation[TARGET_ID].circle.y, 2)) / 1600, aim_position);
		}
		else{
			average_estimately_aim(info, myID, 0.8 * sqrt(pow(info.robotInformation[myID].circle.x - info.robotInformation[TARGET_ID].circle.x, 2) + pow(info.robotInformation[myID].circle.y - info.robotInformation[TARGET_ID].circle.y, 2)) / 1600, aim_position);
		}
	}
	bool is_on_wall = circle_test(info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y, info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x, info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y);
	if (is_on_wall){
		is_circle_move_time_out += 1;
	}
	else{
		int hinder_num = 0;
		if (pow(info.robotInformation[TARGET_ID].circle.x - info.obstacle[0].x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.obstacle[0].y, 2) > pow(info.robotInformation[TARGET_ID].circle.x - info.obstacle[1].x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.obstacle[1].y, 2)){
			hinder_num = 1;
		}
		double my_circle_angle = atan2(-info.robotInformation[myID].circle.y + info.obstacle[hinder_num].y, -info.robotInformation[myID].circle.x + info.obstacle[hinder_num].x);
		double enemy_circle_angle = atan2(-info.robotInformation[TARGET_ID].circle.y + info.obstacle[hinder_num].y, -info.robotInformation[TARGET_ID].circle.x + info.obstacle[hinder_num].x);

		if (is_circle_move_time_out < 150 || ((info.robotInformation[TARGET_ID].weaponTypeName != WT_MissileLauncher || fabs(my_circle_angle - enemy_circle_angle) < 30 / 180 * PI) && (info.robotInformation[TARGET_ID].weaponTypeName == WT_ElectricSaw || pow(info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y, 2) > pow(200, 2) || pow(info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y, 2) < pow(150, 2) || (pow(info.robotInformation[TARGET_ID].circle.x - info.obstacle[0].x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.obstacle[0].y, 2) > pow(250, 2) && pow(info.robotInformation[TARGET_ID].circle.x - info.obstacle[1].x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.obstacle[1].y, 2) > pow(250, 2))))){
			is_circle_move_time_out = 0;
		}
		else{
			is_circle_move_time_out += 1;
		}
	}
	if (is_circle_move_time_out > 200){
		is_circle_move = true;
		average_estimately_aim(info, myID, -0.8, aim_position);
	}
	else{
		is_circle_move = false;
	}

	double target_x = aim_position[0] - info.robotInformation[myID].circle.x;
	double target_y = aim_position[1] - info.robotInformation[myID].circle.y;


	double aim_angle = atan2(target_y, target_x) / PI * 180;//µÃµ½Ãé×¼½Ç¶È
	double weapon_angle = info.robotInformation[myID].weaponRotation;//ÏÖÔÚµÄÎäÆ÷½Ç¶È
	double min_dis = 0;
	bool eff = effective_range(info.robotInformation[myID].circle.x + cos(weapon_angle / 180 * PI) * weapon_rad, info.robotInformation[myID].circle.y + sin(weapon_angle / 180 * PI) * weapon_rad, target_x - cos(weapon_angle / 180 * PI) * weapon_rad, target_y - sin(weapon_angle / 180 * PI) * weapon_rad, min_dis);//ÊÇ·ñ»á´òµ½Öù×ÓÉÏ£¬¿¼ÂÇÅÚËþ³¤¶È


	if (aim_angle > weapon_angle){//µ÷ÕûÃé×¼½Ç¶È
		if (fabs(aim_angle - weapon_angle) < 180){
			order.wturn = 1;
		}
		else{
			order.wturn = -1;
		}
	}
	else{
		if (fabs(aim_angle - weapon_angle) < 180){
			order.wturn = -1;
		}
		else{
			order.wturn = 1;
		}
	}

	if (frames > 5 && pow(target_x, 2) + pow(target_y, 2) < pow(shoot_dis, 2) && fabs(aim_angle - weapon_angle) < 2.5 && eff){
		order.fire = 1;//´ò
	}

	else{
		order.fire = 0;//²»´ò
	}
	if ((pow(info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y, 2) > pow(800, 2) && info.robotInformation[TARGET_ID].weaponTypeName == WT_ElectricSaw && info.robotInformation[myID].remainingAmmo < 41) || (pow(info.robotInformation[TARGET_ID].circle.x - info.robotInformation[myID].circle.x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.robotInformation[myID].circle.y, 2) > pow(500, 2) && info.robotInformation[TARGET_ID].weaponTypeName == WT_ElectricSaw && info.robotInformation[myID].remainingAmmo < 21)){
		order.fire = 0;
	}

	int move[2] = { 0, 0 };

	if (is_circle_move && info.robotInformation[TARGET_ID].weaponTypeName != WT_ElectricSaw){
		int hinder_num = 0;
		if (pow(info.robotInformation[TARGET_ID].circle.x - info.obstacle[0].x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.obstacle[0].y, 2) > pow(info.robotInformation[TARGET_ID].circle.x - info.obstacle[1].x, 2) + pow(info.robotInformation[TARGET_ID].circle.y - info.obstacle[1].y, 2)){
			hinder_num = 1;
		}
		circle_move(hinder_num, info, myID, UFO_route1, move);
		order.run = move[0];
		order.eturn = move[1];
	}
	else{
		UFO_hide(info, myID, move, UFO_route1, bullet_position1);
		if (move[0] == -1){
			order.run = run;
			if (frames % 20 == 0){
				order.run = rand() % 4 + 1;
				run = order.run;
			}
			order.run = 1;
		}
		else{
			order.run = move[0];
			order.eturn = move[1];
		}
		if (frames < 5){
			order.run = 1;
			order.eturn = rand() % 3 - 1;
		}
		if (frames < 20 && info.robotInformation[TARGET_ID].weaponTypeName == WT_Machinegun){
			order.run = 1;
			order.eturn = 0;
			if (frames == 5 || frames == 10 || frames == 15){
				order.eturn = rand() % 3 - 1;
			}
		}
		else if (frames < 30 && info.robotInformation[TARGET_ID].weaponTypeName == WT_Machinegun){
			order.eturn = -1;
		}
	}

	if (not_move && order.run == 0){
		if (frames > 150){
			order.run = 1;
		}
	}
	else if (not_move && order.run != 0){
		not_move = false;
	}

}


void RobotAI::ChooseArmor(weapontypename& weapon, enginetypename& engine, bool a)
{
	engine = ET_UFO;
	weapon = WT_Machinegun;
}


string RobotAI::GetName()
{
	return "Totally Useless";
}

string RobotAI::GetAuthor()
{
	return "Master Yoda";
}

int RobotAI::GetWeaponRed()
{
	return -9;
}
int RobotAI::GetWeaponGreen()
{
	return -0;
}
int RobotAI::GetWeaponBlue()
{
	return -8;
}

int RobotAI::GetEngineRed()
{
	return -58;
}
int RobotAI::GetEngineGreen()
{
	return -67;
}
int RobotAI::GetEngineBlue()
{
	return -111;
}


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info, int myID){
	UFO_route1 = new double*[7776];
	for (int i = 0; i < 7776; i++){
		UFO_route1[i] = new double[2];
	}

	bullet_position1 = new double**[100];
	for (int i = 0; i < 100; i++){
		bullet_position1[i] = new double*[500];
		for (int j = 0; j < 500; j++){
			bullet_position1[i][j] = new double[2];
		}
	}
	srand(time(NULL));

	switch (info.robotInformation[1 - myID].engineTypeName){
	case ET_Spider:
	case ET_Quad:
		TARGET_ENGINE = 1;
		TARGET_HP = 85;
		break;
	case ET_Robotman:
	case ET_Shuttle:
		TARGET_ENGINE = 2;
		TARGET_HP = 90;
		break;
	case ET_AFV:
	case ET_GhostTank:
		TARGET_ENGINE = 3;
		TARGET_HP = 100;
		break;
	case ET_UFO:
	case ET_XCraft:
		TARGET_ENGINE = 4;
		TARGET_HP = 100;
		break;
	}

	switch (info.robotInformation[1 - myID].weaponTypeName){
	case WT_Cannon:
	case WT_Apollo:
		TARGET_WEAPON = 1;
		break;
	case WT_Shotgun:
		TARGET_WEAPON = 2;
		break;
	case WT_RPG:
		TARGET_WEAPON = 3;
		break;
	case WT_Machinegun:
		TARGET_WEAPON = 4;
		break;
	case WT_Prism:
		TARGET_WEAPON = 5;
		break;
	case WT_Tesla:
		TARGET_WEAPON = 6;
		break;
	case WT_PlasmaTorch:
		TARGET_WEAPON = 7;
		break;
	case WT_MissileLauncher:
		TARGET_WEAPON = 8;
		break;
	case WT_ElectricSaw:
	case WT_MineLayer:
	case WT_GrenadeThrower:
		TARGET_WEAPON = 9;
		break;
	}
}

void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info, int myID)
{
	for (int i = 0; i < 7776; i++)
	{
		delete[] UFO_route1[i]; //ÏÈ³·ÏúÖ¸ÕëÔªËØËùÖ¸ÏòµÄÊý×é
	}
	delete[] UFO_route1;

	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 50; j++){
			delete[] bullet_position1[i][j];
		}
		delete[] bullet_position1[i];
	}
	delete[] bullet_position1;
}



void RobotAI::onSomeoneFire(int fireID)
{

}


void RobotAI::onHit(int launcherID, bullettypename btn)
{

}