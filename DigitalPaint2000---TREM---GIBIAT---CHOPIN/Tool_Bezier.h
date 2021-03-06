#pragma once


/*
	Handles mouse press events passed onto the Rect tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
int nbPoints;
int storedPoint;
int TMax = 5000;
int rayon = 10;
Colour SelectionControle = {0,1, 0.97}; //Cyan

void Tool_Bezier::DrawCircleAroundControlPoint(int index,Colour c) {
	int x, y;
	for (int t = 0; t < TMax; t++)
	{
		y = polygonControl[index].y + rayon * sin(t * 2 * M_PI / TMax);
		x = polygonControl[index].x + rayon * cos(t * 2 * M_PI / TMax);
		currentCanvas.SetPixelColour(x, y, c);
	}
}
bool Tool_Bezier::Pressed(int button, int state, int x, int y) {
	if (!Select) {
		if (currentCanvas.checkInside(x, y)) {
			// convert mouse position into canvas coordinates
			int cx = (x - currentCanvas.xOffset) / currentCanvas.zoom;
			int cy = (y - currentCanvas.yOffset) / currentCanvas.zoom;
			// remember the start mouse position if this is start of a drag
			if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown && firstPick) {
				isMouseDown = true;
				startMouseX = cx;
				startMouseY = cy;
				departX = cx;
				departY = cy;
				ListeSommets.clear();
				ListeSommets.push_back({ startMouseX, startMouseY });
				polygonControl.push_back({ startMouseX,startMouseY });
				for (int t = 0; t < TMax; t++)
				{
					y = cy + rayon * sin(t * 2 * M_PI / TMax);
					x = cx + rayon * cos(t * 2 * M_PI / TMax);
					currentCanvas.SetPixelColour(x, y, selection);
				}
				firstPick = false;
				nbPoints = 0;
				return true;
			}
			if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {

				// get the rect coordinates
				float minX = startMouseX;
				float maxX = cx;
				float minY = startMouseY;
				float maxY = cy;
				float difX = abs(minX - maxX);
				float difY = abs(minY - maxY);
				float CoefD = ((maxY - minY) / (maxX - minX));
				float b = minY - CoefD * minX;
				currentCanvas.DrawALine(startMouseX, startMouseY, cx, cy, selectedColour);
				//currentCanvas.SetPixelColour(cx, cy, selectedColour);


				ListeCotes.push_back({ startMouseX,startMouseY,cx,cy,CoefD, b });
				ListeSommets.push_back({ cx, cy });
				if (Tool_Bezier::duplicate) {
					polygonControl.push_back({ cx,cy });
					Tool_Bezier::duplicate = false;
					nbPoints++;
				}
				polygonControl.push_back({ cx,cy });
				for (int t = 0; t < TMax; t++)
				{
					y = cy + rayon * sin(t * 2 * M_PI / TMax);
					x = cx + rayon * cos(t * 2 * M_PI / TMax);
					currentCanvas.SetPixelColour(x, y, selection);
				}
				nbPoints++;
				/*if (nbPoints >= 3 && BezierEnded) {
					EndBezier();
				}*/

				if (B1) {
					if (ListeSommets.size() == 3) {
						currentCanvas.ResetPixelsColour();
						Bezier1();

						firstBezier = false;
					}
					if (!firstBezier) {
						currentCanvas.ResetPixelsColour();
						Bezier1();
					}
				}
				else {
					if (ListeSommets.size() == 4) {
						currentCanvas.ResetPixelsColour();
						Bezier2();
						firstBezier = false;
					}
					if (!firstBezier && (ListeSommets.size() - 4) % 3 == 0) {
						currentCanvas.ResetPixelsColour();
						Bezier2();
					}
				}


				
				
				int ind = 0;
				for (ind; ind < polygonControl.size() - 1; ind++) {
					DrawCircleAroundControlPoint(ind, selection);
					currentCanvas.DrawALine(polygonControl[ind].x, polygonControl[ind].y, polygonControl[ind + 1].x, polygonControl[ind + 1].y, selectedColour);

					
				}
				DrawCircleAroundControlPoint(ind, selection);

			}
			startMouseX = cx;
			startMouseY = cy;
			isMouseDown = false;
			return true;
		}
		return false;
	}
	else {
		if (currentCanvas.checkInside(x, y)) {
			int cx = (x - currentCanvas.xOffset) / currentCanvas.zoom;
			int cy = (y - currentCanvas.yOffset) / currentCanvas.zoom;
			int i = 0;
			if (!drop) {
				if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {
					int size = polygonControl.size();
					for (int i = 0; i < size; i++) {
						if (abs(polygonControl[i].x - cx) < 10 && abs(polygonControl[i].y - cy) < 10) {
							storedPoint = i;
							DrawCircleAroundControlPoint(i, SelectionControle);
							if (duplicate) {
								currentCanvas = NewCanvas(currentCanvas.width, currentCanvas.height, 0, 0);
								DuplicateControle();
								int ind = 0;
								for (ind; ind < polygonControl.size() - 1; ind++) {
									DrawCircleAroundControlPoint(ind, selection);
									currentCanvas.DrawALine(polygonControl[ind].x, polygonControl[ind].y, polygonControl[ind + 1].x, polygonControl[ind + 1].y, selectedColour);
								}
								DrawCircleAroundControlPoint(ind, selection);
								duplicate = false;

							}
							break;
						}
					}

				}


				return true;
			}
			else {

				if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {
					polygonControl[storedPoint] = { cx,cy };
					currentCanvas.ResetPixelsColour();
					if (B1)
						Bezier1();
					else
						Bezier2();

					int ind = 0;
					for (ind; ind < polygonControl.size() - 1; ind++) {
						DrawCircleAroundControlPoint(ind, selection);
						currentCanvas.DrawALine(polygonControl[ind].x, polygonControl[ind].y, polygonControl[ind + 1].x, polygonControl[ind + 1].y, selectedColour);
					}
					DrawCircleAroundControlPoint(ind, selection);

				}
				return true;
			}
		}

		return false;
	}
}

void Tool_Bezier::Bezier2() {

	

	ListeSommetsCurve = {};



	std::list<std::array<Tuple, 4>> ListeSommetsBezier;

	int i = 0;

	std::array<Tuple, 4> SommetsBezier{};
	for (auto const& it : polygonControl) {


		SommetsBezier[i] = it;

		if (i == 3) {
			ListeSommetsBezier.push_back(SommetsBezier);
			SommetsBezier[0] = SommetsBezier[3];
			//SommetsBezier[1] = SommetsBezier[3];
			i = 0;
		}

		i++;

	}




	for (auto const& it : ListeSommetsBezier) {

		SommetsBezier = it;

		ListeSommetsCurve.push_back(SommetsBezier[0]);

		for (int i = 1; i < step; i++) {

			Tuple a1 = Bary(i, step, SommetsBezier[0], SommetsBezier[1]);
			Tuple a2 = Bary(i, step, SommetsBezier[1], SommetsBezier[2]);
			Tuple a3 = Bary(i, step, SommetsBezier[2], SommetsBezier[3]);

			Tuple a4 = Bary(i, step, a1, a2);
			Tuple a5 = Bary(i, step, a2, a3);
			Tuple a6 = Bary(i, step, a4, a5);
			ListeSommetsCurve.push_back(a6);

		}
		ListeSommetsCurve.push_back(SommetsBezier[3]);

	}

	drawCurve();
}

void Tool_Bezier::showStep() {

	currentCanvas.ResetPixelsColour();
	Redraw();

	std::vector<Tuple> SommetsBezier{};
	std::vector<Tuple> sommetsTemp{};
	for (auto const& it : polygonControl) {

		SommetsBezier.push_back(it);

	}

	Colour color = { 0.0f, 0.1f, 0.1f };

	Tuple temp;
	sommetsTemp = SommetsBezier;
	for (int j = 0; j < SommetsBezier.size(); j++) {


		temp = sommetsTemp[0];
		for (int k = 1; k < sommetsTemp.size() - j; k++) {


			sommetsTemp[k - 1] = Bary(stepi, step, temp, sommetsTemp[k]);
			currentCanvas.DrawALine(temp.x, temp.y, sommetsTemp[k].x, sommetsTemp[k].y, color);
			temp = sommetsTemp[k];
			

		}
		color = { 0.0f, color.g + (1.0f / SommetsBezier.size()) , color.b + (1.0f / SommetsBezier.size()) };

	}

}



void Tool_Bezier::Bezier1() {
	
	ListeSommetsCurve = {};

	std::list<std::array<Tuple, 4>> ListeSommetsBezier;

	int i = 0;

	std::vector<Tuple> SommetsBezier{};
	std::vector<Tuple> sommetsTemp{};
	for (auto const& it : polygonControl) {

		SommetsBezier.push_back(it);

	}

	Tuple temp;
	ListeSommetsCurve.push_back(SommetsBezier[0]);
	for (int i = 1; i < step; i++) {

		sommetsTemp = SommetsBezier;
		for (int j = 0; j < SommetsBezier.size(); j++) {


			temp = sommetsTemp[0];
			for (int k = 1; k < sommetsTemp.size() - j; k++) {


				sommetsTemp[k - 1] = Bary(i, step, temp, sommetsTemp[k]);
				temp = sommetsTemp[k];

			}

		}
		ListeSommetsCurve.push_back(sommetsTemp[0]);

	}
	ListeSommetsCurve.push_back(SommetsBezier.back());

	drawCurve();
}

void Tool_Bezier::BezierEtienne() {

	ListeSommetsCurve = {};

	nIter = 10;

	std::list<std::array<Tuple, 4>> ListeSommetsBezier;

	std::array<Tuple, 4> SommetsBezier{};
	int i = 0;
	for (auto const& it : polygonControl) {
		
		SommetsBezier[i] = it;
		i++;

	}

	ListeSommetsBezier.push_back(SommetsBezier);

	i = 0;
	std::list<std::array<Tuple, 4>>::iterator it = ListeSommetsBezier.begin();
	while(it != ListeSommetsBezier.end()){
		SommetsBezier = *it;
	
		Tuple a1 = Mid(SommetsBezier[0], SommetsBezier[1]);
		Tuple a2 = Mid(SommetsBezier[1], SommetsBezier[2]);
		Tuple a3 = Mid(SommetsBezier[2], SommetsBezier[3]);

		Tuple a4 = Mid(a1, a2);
		Tuple a5 = Mid(a2, a3);

		currentCanvas.DrawALine(a1.x, a1.y, a4.x, a4.y, selectedColour);
		currentCanvas.DrawALine(a4.x, a4.y, a5.x, a5.y, selectedColour);
		currentCanvas.DrawALine(a5.x, a5.y, a3.x, a3.y, selectedColour);
		


		if (i < nIter) {
			ListeSommetsBezier.push_back({ SommetsBezier[0], a1, a4, a5 });
			ListeSommetsBezier.push_back({ a4, a5, a3, SommetsBezier[3] });
		}
		

		std::list<std::array<Tuple, 4>>::iterator pos = ListeSommetsBezier.erase(it);
		it = pos;
		i++;

	}
	
}

void Tool_Bezier::drawCurve() {
	
	Tuple temp;
	int i = 0;
	for (auto const& it : ListeSommetsCurve) {

		if (i != 0) {
			currentCanvas.DrawALine(temp.x, temp.y, it.x, it.y, { 0.0f, 0.0f, 0.0f });
		}

		temp = it;
		i++;

	}

	
}


Tuple Tool_Bezier::Mid(Tuple a, Tuple b) {

	return { (a.x + b.x) / 2, (a.y + b.y) / 2};
}

Tuple Tool_Bezier::Bary(int iStep, int step,  Tuple a, Tuple b) {
	float iStepf = static_cast<float>(iStep);
	float stepf = static_cast<float>(step);

	return { (a.x + static_cast<int>(((iStepf / stepf) * (b.x - a.x)))), (a.y + static_cast<int>(((iStepf / stepf) * (b.y - a.y))))};
}


void Tool_Bezier::SuppressionControle1() {
	if (polygonControl.size() > 4) {
		auto iter = polygonControl.begin() + storedPoint;
		polygonControl.erase(iter);
		currentCanvas = NewCanvas(currentCanvas.width, currentCanvas.height, 0, 0);
		int ind = 0;
		for (ind; ind < polygonControl.size() - 1; ind++) {
			DrawCircleAroundControlPoint(ind, selection);
			currentCanvas.DrawALine(polygonControl[ind].x, polygonControl[ind].y, polygonControl[ind + 1].x, polygonControl[ind + 1].y, selectedColour);
		}
		DrawCircleAroundControlPoint(ind, selection);
		Bezier1();
	}
}

void Tool_Bezier::SuppressionControle2() {
	if (polygonControl.size() > 4) {

		if ((storedPoint + 1) % 3 == 0) {
			auto iter = polygonControl.begin() + storedPoint - 1;
			polygonControl.erase(iter, iter + 3);
		}
		if ((storedPoint + 1) % 3 == 1) {
			auto iter = polygonControl.begin() + storedPoint - 2;
			polygonControl.erase(iter, iter + 3);
		}
		if ((storedPoint + 1) % 3 == 2) {
			auto iter = polygonControl.begin() + storedPoint;
			polygonControl.erase(iter, iter + 3);
		}
		currentCanvas = NewCanvas(currentCanvas.width, currentCanvas.height, 0, 0);
		int ind = 0;
		for (ind; ind < polygonControl.size() - 1; ind++) {
			DrawCircleAroundControlPoint(ind, selection);
			currentCanvas.DrawALine(polygonControl[ind].x, polygonControl[ind].y, polygonControl[ind + 1].x, polygonControl[ind + 1].y, selectedColour);
		}
		DrawCircleAroundControlPoint(ind, selection);
		Bezier2();
	}
}

void Tool_Bezier::DuplicateControle() {
	polygonControl.insert(polygonControl.begin() + storedPoint + 1,polygonControl[storedPoint]);
	nbPoints++;
	if (B1)
		Bezier1();
	else
		Bezier2();
}

void Tool_Bezier::Redraw() {
	if (B1)
		Bezier1();
	else
		Bezier2();
	drawCurve();
	int ind = 0;
	for (ind; ind < polygonControl.size() - 1; ind++) {
		DrawCircleAroundControlPoint(ind, selection);
		currentCanvas.DrawALine(polygonControl[ind].x, polygonControl[ind].y, polygonControl[ind + 1].x, polygonControl[ind + 1].y, selectedColour);
	}
	DrawCircleAroundControlPoint(ind, selection);
}

int gaucheMax = 0, droiteMax = 0, hautMax = 0, basMax = 0;
int gaucheActuel = 0, droiteActuel = 0, hautActuel = 0, basActuel = 0;
int xCentre = 0, yCentre = 0;

void Tool_Bezier::Scale(float const & pas)
{
	if (!dejaScale)
	{
		gaucheMax = 0, droiteMax = 0, hautMax = 0, basMax = 0;
		gaucheActuel = 0, droiteActuel = 0, hautActuel = 0, basActuel = 0;

		for (int i = 0; i < polygonControl.size() - 1; i++)
		{
			if (gaucheActuel < polygonControl[i + 1].x)
			{
				gaucheMax = gaucheActuel;
			}
		}

		for (int i = 0; i < polygonControl.size() - 1; i++)
		{
			if (droiteActuel > polygonControl[i + 1].x)
			{
				droiteMax = droiteActuel;
			}
		}

		for (int i = 0; i < polygonControl.size() - 1; i++)
		{
			if (hautActuel < polygonControl[i + 1].y)
			{
				hautMax = hautActuel;
			}
		}

		for (int i = 0; i < polygonControl.size() - 1; i++)
		{
			if (basActuel > polygonControl[i + 1].y)
			{
				basMax = basActuel;
			}
		}

		xCentre = (droiteMax + gaucheActuel) / 2;
		yCentre = (hautMax + basMax) / 2;
	}


	for (auto & ptControl : polygonControl)
	{
		float minX = xCentre;
		float maxX = ptControl.x;
		float minY = yCentre;
		float maxY = ptControl.y;
		float difX = abs(minX - maxX);
		float difY = abs(minY - maxY);

		float y;
		float x;
		float CoefD = ((maxY - minY) / (maxX - minX));
		float b = minY - CoefD * minX;

		if (difX > difY)
		{
			x = pas;
			maxX += pas;
			y = CoefD * (maxX)  + b;
			maxY = y;
		}
		else
		{
			if (difX == 0)
			{
				x = maxX + pas;
				maxX = x;
			}
			else
			{
				maxY += pas;
				x = ((maxY) - b) / CoefD;
				maxX = x;

			}
		}
		ptControl.x = maxX;
		ptControl.y = maxY;
	}

	currentCanvas.ResetPixelsColour();
	int x = 0;
	int y = 0;
	int ind = 0;
	for (ind; ind < Tool_Bezier::polygonControl.size() - 1; ind++) {

		for (int t = 0; t < TMax; t++)
		{
			y = Tool_Bezier::polygonControl[ind].y + rayon * sin(t * 2 * M_PI / TMax);
			x = Tool_Bezier::polygonControl[ind].x + rayon * cos(t * 2 * M_PI / TMax);
			currentCanvas.SetPixelColour(x, y, { 0,0,0 });
		}
		currentCanvas.DrawALine(Tool_Bezier::polygonControl[ind].x, Tool_Bezier::polygonControl[ind].y, Tool_Bezier::polygonControl[ind + 1].x, Tool_Bezier::polygonControl[ind + 1].y, selectedColour);
	}
	for (int t = 0; t < TMax; t++)
	{
		y = Tool_Bezier::polygonControl[ind].y + rayon * sin(t * 2 * M_PI / TMax);
		x = Tool_Bezier::polygonControl[ind].x + rayon * cos(t * 2 * M_PI / TMax);
		currentCanvas.SetPixelColour(x, y, { 0,0,0 });
	}
	Tool_Bezier::Bezier1();
}



