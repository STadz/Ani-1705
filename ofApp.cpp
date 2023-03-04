#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	//setBgColor();
	
	image_settings.accurate;
	
	//GUI
	ofxGuiSetFont("RobertoSans-Regular.ttf", 10, true, true, 96);
	
	//Hierarchy
	gui_hierarchy.setup();
	gui_hierarchy.setName("Scene Hierarchy");
	gui_hierarchy.minimize();

	//Layers
	gui_layers.setup();
	gui_layers.setName("Layers");
	gui_layers.minimize();

	//File Menu
	gui_file_menu.setup();
	gui_file_menu.setName("File");
	gui_file_menu.minimize();
	
	//3D World
	gui_3D_menu.setup();
	gui_3D_menu.setName("3D Scene");
	gui_3D_menu.minimize();

	//Image menu
	gui_image_menu.setup();
	gui_image_menu.setName("Image");
	gui_image_menu.minimize();

	//Light menu
	gui_light_menu.setup();
	gui_light_menu.setName("Light");
	gui_light_menu.minimize();
	gui_light_menu.add(sun_menu_color_amb.setup("Ambient Color", ofVec3f(sun_color_amb.r, sun_color_amb.g, sun_color_amb.b), ofVec3f(0, 0, 0), ofVec3f(255, 255, 255)));
	gui_light_menu.add(sun_menu_color_diff.setup("Diffuse Color", ofVec3f(sun_color_diff.r, sun_color_diff.g, sun_color_diff.b), ofVec3f(0, 0, 0), ofVec3f(255, 255, 255)));
	gui_light_menu.add(sun_intensity.setup("Intensity", 0.3f, 0, 1));

	sun_menu_color_amb.getFloatSlider("x").addListener(this, &ofApp::SunColor_AmbRValueChanged);
	sun_menu_color_amb.getFloatSlider("y").addListener(this, &ofApp::SunColor_AmbGValueChanged);
	sun_menu_color_amb.getFloatSlider("z").addListener(this, &ofApp::SunColor_AmbBValueChanged);

	sun_menu_color_diff.getFloatSlider("x").addListener(this, &ofApp::SunColor_DiffRValueChanged);
	sun_menu_color_diff.getFloatSlider("y").addListener(this, &ofApp::SunColor_DiffGValueChanged);
	sun_menu_color_diff.getFloatSlider("z").addListener(this, &ofApp::SunColor_DiffBValueChanged);

	sun_intensity.addListener(this, &ofApp::SunIntensityValueChanged);

	//light_menu_color.addListener(this, &ofApp::SunColorValueChanged);

	//Camera menu	
	gui_camera_menu.setup();
	gui_camera_menu.setName("Camera");
	gui_camera_menu.add(cam_menu_pos.setup("Position", ofVec3f(0, 0, -50), ofVec3f(-500, -500, -5000), ofVec3f(500, 500, 5000)));
	gui_camera_menu.add(cam_menu_rot.setup("Rotation", ofVec3f(0, 0, 0), ofVec3f(-360, -360, -360), ofVec3f(360, 360, 360)));
	gui_camera_menu.add(cam_menu_fov.setup("Fielf of View", 60.0f, 0.1f, 179.9f));
	gui_camera_menu.add(cam_menu_clip_near.setup("Near Clip", 50.0f, 0.1f, 500.0f));
	gui_camera_menu.add(cam_menu_clip_far.setup("Far Clip", 1000.0f, 20.0f, 2000.0f));
	gui_camera_menu.add(cam_menu_dolly_speed.setup("dolly speed", 0.0f, -0.0005f, 0.0005f));
	gui_camera_menu.add(cam_menu_zoom_speed.setup("zoom speed", 0.0f, -0.005f, 0.005f));
	cam_menu_dolly_speed.addListener(this, & ofApp::camSetZoom);
	cam_menu_zoom_speed.addListener(this, &ofApp::camSetDolly);


	//cam_menu_dolly_speed
	cam_menu_pos.getFloatSlider("x").addListener(this, &ofApp::camPosSliderXChanged);
	cam_menu_pos.getFloatSlider("y").addListener(this, &ofApp::camPosSliderYChanged);
	cam_menu_pos.getFloatSlider("z").addListener(this, &ofApp::camPosSliderZChanged);
	cam_menu_fov.addListener(this, &ofApp::camFovSliderChanged);
	cam_menu_clip_near.addListener(this, &ofApp::camClipNearSliderChanged);
	cam_menu_clip_far.addListener(this, &ofApp::camClipFarSliderChanged);
	gui_camera_menu.minimize();

	//Send menu elements in a list
	gui_menu_list.push_back(&gui_file_menu);
	gui_menu_list.push_back(&gui_3D_menu);
	gui_menu_list.push_back(&gui_image_menu);
	gui_menu_list.push_back(&gui_camera_menu);
	gui_menu_list.push_back(&gui_light_menu);

	//Set GUI to position
	resetGUI();

	//Load default demonstration model
	character_model.loadModel("perso3D.obj", true);
	character_model.setPosition(0, 0, 0);	
	//character_model.disableMaterials();

	//Load background models
	float center_x = ofGetWidth() / 2.0f;
	float center_y = ofGetHeight() / 2.0f;
	
	//Set default shader
	shader_default.load("lambert_330_vs.glsl", "lambert_330_fs.glsl");

	//Load default camera
	cam_target = { 0.0f, 0.0f, 0.0f };
	cam_clip_n = 50.0f;
	cam_clip_f = 1000.f;
	cam_fov = 60.0f;

	cam_default->setFov(cam_fov);
	cam_default->setFarClip(cam_clip_f);
	cam_default->setNearClip(cam_clip_n);
	cam_default->setGlobalPosition(glm::vec3(0, 0, -50));
	cam_default->setupPerspective(true, cam_fov, cam_clip_n, cam_clip_f, glm::vec2(0, 0));

	std::cout << cam_default->getUpDir() << " is cam up vec" << endl;

	//Sun Light
	sun->setDirectional();
	
	//Set first layer
	layer_2D bg;
	bg.index = 0;
	bg.name = "Background";
	gui_image_menu.add(layer_new.setup("New layer", 20, 20));
	gui_image_menu.add(layer_del.setup("Delete Selected layers",20, 20));
	layer_new.addListener(this, &ofApp::CreateNewLayer);
	layer_del.addListener(this, &ofApp::DeleteSelectedLayers);




	/*/These are tests/examples
	// Add the listener function to the slider
	floatSlider.addListener(this, &ofApp::FloatSliderValueChanged);
	button.setSize(200, 10);

	gui_file_menu.add(intSlider.setup("int slider", 64, 3, 64));
	gui_file_menu.add(floatSlider.setup("float slider", 30.0, 0.0, 300.0));

	gui_file_menu.add(toggle.setup("toggle", false));
	gui_file_menu.add(button.setup("Save",250, 20));
	gui_file_menu.add(label.setup("label", "THIS IS A LABEL"));

	gui_file_menu.add(intField.setup("int field", 100, 0, 100));
	gui_file_menu.add(floatField.setup("float field", 100.0, 0.0, 100.0));
	gui_file_menu.add(textField.setup("text field", "text"));

	gui_image_menu.add(vec2Slider.setup("vec2 slider", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	gui_image_menu.add(vec3Slider.setup("vec3 slider", ofVec3f(100, 150, 90), ofVec3f(0, 0, 0), ofVec3f(255, 255, 255)));
	gui_image_menu.add(vec4Slider.setup("vec4 slider", ofVec4f(0, 0, 0, 0), ofVec4f(0, 0, 0, 0), ofVec4f(255, 255, 255, 255)));

	vec2Slider.setHeaderBackgroundColor(ofColor::darkSlateBlue);
	vec4Slider.setHeaderBackgroundColor(ofColor::darkSlateBlue);
	*/
}

//--------------------------------------------------------------
void ofApp::update(){	
	// position au centre de la fenêtre d'affichage
	float center_x = ofGetWidth()  / 2.0f;
	float center_y = ofGetHeight() / 2.0f;

	/* transformation du modele
	//character_model.setScale(1, 1, 1);
	//character_model.setPosition(center_x, center_y + 150, 0);

	if (true) { 
		character_model.setRotation(0, 0.0f, ofGetFrameNum() * 0.3f, 0.0f, 1.0f); 
	}
	*/


	if (mouse_left_held) {
		ofSetColor(255, 0, 255, 255);
		//ofLine(mouse_click_pos.x, mouse_click_pos.y, (float)ofGetMouseX(), (float)ofGetMouseY());
	}




	// configuration de la lumière
	sun->setup();
	sun->setPointLight();
	sun->setDiffuseColor(sun_color_diff);
	sun->setAmbientColor(sun_color_amb);	
	sun->setGlobalPosition(center_x, center_y, 255.0f);

	// passer les attributs uniformes du shader
	shader_default.begin();
	shader_default.setUniform3f("color_ambient", (float)sun_color_amb.r/255, (float)sun_color_amb.g / 255, (float)sun_color_amb.b / 255);
	shader_default.setUniform3f("color_diffuse", sun_color_diff.r* sun_intensity_value / 255, sun_color_diff.g* sun_intensity_value / 255, sun_color_diff.b* sun_intensity_value / 255);
	shader_default.setUniform3f("light_position", sun->getGlobalPosition());
}

//--------------------------------------------------------------
void ofApp::draw(){

	setBgColor();

	//Draw images in image list
	if (images_list.size() > 0) {
		for (int i = 0; i < images_list.size(); i++) {
			images_list[i].draw(50.0f * i, 50 * i, 250, ofGetWidth(), ofGetHeight());
		}
	}


	cam_default->begin();

	// activer l'occlusion en profondeur
	ofEnableDepthTest();

	// activer l'éclairage dynamique
	ofEnableLighting();
		
	if (meshes_list.size() > 0) {
		for (int i = 0; i < meshes_list.size(); i++) {
			meshes_list[i].draw();
		}
	}

	// activer la lumière	
	sun->enable();

	// activer le shader
	shader_default.begin();

	//Background characters
	if (bg_models.size() > 0) {
		for (int i = 0; i < bg_models.size(); i++) {
			bg_models[i].draw(OF_MESH_FILL);
		}
	}

	// dessiner l'objet
	character_model.draw(OF_MESH_FILL);
	
	// désactiver le shader
	shader_default.end();
	
	//Dessiner le 2D
	if (mouse_left_held) {
		ofSetColor(255, 0, 255, 255);
		ofDrawLine(mouse_click_pos.x, mouse_click_pos.y, (float)ofGetMouseX(), (float)ofGetMouseY());
	}


	// désactiver la lumière
	sun->disable();

	// désactiver l'éclairage dynamique
	ofDisableLighting();

	// désactiver l'occlusion en profondeur
	ofDisableDepthTest();

	cam_default->end();

	// dessiner l'interface graphique
	resetGUI();

	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	std::cout << key << endl;
	//Modifiers keys
	if (key == ofKey::OF_KEY_CONTROL && !key_ctrl_held)  { key_ctrl_held  = true; key_ctrl_held = true; return; }		//Ctrl
	if (key == ofKey::OF_KEY_ALT     && !key_alt_held)   { key_alt_held   = true; key_ctrl_held = true; return; }		//Alt
	if (key == ofKey::OF_KEY_SHIFT   && !key_shift_held) { key_shift_held = true; key_ctrl_held = true; return; }		//Shift
	

	//Change Gizmo mode
	if (key == 'w' && gizmo_mode != GizmoMode::translate) { gizmo_mode = GizmoMode::translate; return; }
	if (key == 'e' && gizmo_mode != GizmoMode::rotate)    { gizmo_mode = GizmoMode::rotate; return; }
	if (key == 'r' && gizmo_mode != GizmoMode::scale)     { gizmo_mode = GizmoMode::scale; return; }
	
	//Camera Presets
	if(key == VK_NUMPAD4) { std::cout << "numpad 4 " << VK_NUMPAD4 << endl; }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	std::cout << "gizmo= " << gizmo_mode << endl;
	
	//Modifiers keys
	if (key == ofKey::OF_KEY_CONTROL && key_ctrl_held)  { key_ctrl_held  = false; setKeyModifier(); return; }		//Ctrl
	if (key == ofKey::OF_KEY_ALT	 && key_alt_held)   { key_alt_held   = false; setKeyModifier(); return; }		//Alt
	if (key == ofKey::OF_KEY_SHIFT   && key_shift_held) { key_shift_held = false; setKeyModifier(); return; }		//Shift

	//Toggle GUI on and off
	if (key == '~') { is_gui_shown = !is_gui_shown; }

	//Delete selection from hierarchy
	if (key == ofKey::OF_KEY_CONTROL) {
		deleteHierarchySelection();
	}

	if (key == ofKey::OF_KEY_PAGE_DOWN) {
		cam_default->setGlobalPosition(glm::vec3(0, 0, -50));
		cam_default->setupPerspective(false, cam_fov, cam_clip_n, cam_clip_f, glm::vec2(0, 0));
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
	/*

	*/
	//std::cout << "mouse delta: " << mouse_delta << ", to position " << x << ", " << y << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	std::cout << "mouse drag button "<< button << "at " << x << ", " << y << endl;

	if (mouse_wheel_held) {
		mouse_delta = ofVec2f(ofGetMouseX() - ofGetPreviousMouseX(), ofGetMouseY() - ofGetPreviousMouseY());
		float _x = cam_default->getGlobalPosition().x + mouse_delta.x;
		float _y = cam_default->getGlobalPosition().y + mouse_delta.y;
		float _z = cam_default->getGlobalPosition().z;

		cam_default->setGlobalPosition(_x, _y, _z);     //move(mouse_delta.x, mouse_delta.y, 0);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//Mouse wheel
	if (button == 1 && !mouse_any_held) {
		if (!mouse_wheel_held) {
			mouse_click_pos = ofVec2f(ofGetMouseX() - cam_default->getGlobalPosition().x, ofGetMouseY() - cam_default->getGlobalPosition().y);
		}

		mouse_wheel_held = true;
		mouse_any_held = true;
		std::cout << "mouse click pos " << mouse_click_pos.x << ", " << mouse_click_pos.y << endl;
		return;
	}

	//Left button
	if (button == 0 && !mouse_any_held) {
		if (!mouse_left_held) {
			mouse_click_pos = ofVec2f(ofGetMouseX() - cam_default->getGlobalPosition().x, ofGetMouseY() - cam_default->getGlobalPosition().y);
		}
		mouse_left_held = true;
		mouse_any_held = true;
		std::cout << "mouse click pos " << mouse_click_pos.x << ", " << mouse_click_pos.y << endl;
		return;
	}

	//Right button
	if (button == 2 && !mouse_any_held) {
		if (!mouse_right_held) {
			mouse_click_pos = ofVec2f(ofGetMouseX() + cam_default->getGlobalPosition().x, ofGetMouseY() + cam_default->getGlobalPosition().y);
		}
		mouse_right_held = true;
		mouse_any_held = true;
		std::cout << "mouse click pos " << mouse_click_pos.x << ", " << mouse_click_pos.y << endl;
		return;
	}


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	//Mouse wheel

	if (button == 1) {
		mouse_wheel_held = false;
	} 

	//Left button
	if (button == 0) {
		mouse_left_held = false;
	}

	//Right button
	if (button == 2) {
		mouse_right_held = false;
	}

	mouse_any_held = mouse_wheel_held || mouse_left_held || mouse_right_held;

	std::cout << "mouse release with button "<< button << "at " << x << ", " << y << endl;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	std::cout << "mouse entered at " << x << ", " << y << endl;
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){	
	
	int _scrollDir = scrollY > 0 ? 1 : -1;
	std::cout << _scrollDir << endl;

	//glm::vec3 p = cam_default->getPosition();
	cam_default->dolly(scrollY* zoom_scroll_speed);

	//cam_default->setPosition(glm::vec3(p.x, p.y, _scrollDir/100));
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
	std::cout << "mouse exited at " << x << ", " << y << endl;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	std::cout << "window resized to " << w << "x" << h << endl;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	std::cout << "dragged "<< dragInfo.files.size() << "at position " << dragInfo.position << endl;

	for each (ofFile f in dragInfo.files)
	{
		std::string fpath = f.getAbsolutePath(); 
		std::string fname = f.getFileName();
		std::string ftype = ofUTF8Substring(fname, fname.length() - f.getExtension().length() -1 , fname.length());

		std::cout << "file name is " << fname << " path is " << fpath << " extension is "<< ftype << endl;
	
		//Image file
		if ((ftype == JPG) || (ftype == JPEG) || (ftype == PNG) || (ftype == BMP)) {
			if (ofLoadImage(image_texture, fpath, image_settings)) {
				//Get image
				ofImage _image = ofImage(fpath, image_settings);

				//Put image in images vector
				images_list.push_back(_image);

				//Make a struct for hierarchy panel
				hierarchy_element* e = new hierarchy_element();
				e->index = gui_hierarchy_selection.size();
				e->name = fname;

				//Make a new toggle button and add it to the hierarchy
				ofxToggle* t = new ofxToggle();
				gui_hierarchy.add(t->setup(e->name, false));

				//Add a listener to the new toggle
				t->addListener(this, &ofApp::updateHierarchySelection);

				//Make added element selected by default
				gui_hierarchy_selection.push_back(*e);
				std::cout << fname << " is loaded " << endl;
			}			
		}
	
		//3D model file
		if ((ftype == OBJ) || (ftype == FBX) || (ftype == CAD)) {
			ofxAssimpModelLoader m;
			
			if (m.loadModel(ofBufferFromFile(fpath), false)) {
				
				m.getMesh(0);

				meshes_list.push_back(m.getMesh(0));

				//std::cout << fname << " is loaded " << endl;
			}
		}	
	}
}

//--------------------------------------------------------------
void ofApp::setBgColor() {
	switch (background_mode)
	{
	case ofApp::solid:
		ofSetBackgroundColor(background_color);
		break;
	case ofApp::image:
		break;
	case ofApp::skybox:
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::setKeyModifier() {
	key_modifier_any = key_ctrl_held || key_alt_held || key_shift_held;
}

//--------------------------------------------------------------
void ofApp::FloatSliderValueChanged(float& value)
{
	//Get normalized value
	float val = value / floatSlider.getMax();
	cout << "Slider value changed: " << value << " representing " << val*100 << "%" << endl;
	floatSlider.setFillColor(ofColor(255-(val*255),val * 210, 0, 255));
}

//--------------------------------------------------------------
void ofApp::resetGUI() {
	//Set UI Menu position
	float _w = 0;
	float _wtext;

	//Update menu items
	for each (ofxPanel * m in gui_menu_list)
	{
		//Set colors to preset
		m->setBorderColor(menu_color_border);
		m->setBackgroundColor(menu_color_bg);
		m->setHeaderBackgroundColor(menu_color_headerbg);

		//Set width depending if minimized or expanded
		if (m->isMinimized()) {
			_wtext = m->getName().size() * 10 + 10;
			m->setShape(_w, 0, _wtext, 20);
			m->minimizeAll();
		}
		else {
			m->setShape(_w, 0, 250, 20);
		}

		//Set Position
		if (m->getPosition() != glm::vec3(_w, 0, 0)) {
			m->setPosition(_w, 0);
		}

		//Draw the UI element
		m->draw();

		//Update width for next element
		_w += m->getWidth();		
	}

		//Stick hierarchy panel on top-right corner of screen
		gui_hierarchy.setPosition(ofGetWidth() - gui_hierarchy.getWidth(), 0);

		//Draw the scene hierarchy Gui
		gui_hierarchy.draw();

		//Draw the Layers Gui below scene hierarchy
		gui_layers.setPosition(ofGetWidth() - gui_layers.getWidth(), gui_hierarchy.getHeight());
		gui_layers.draw();
}

//--------------------------------------------------------------
void ofApp::deleteHierarchySelection() {
	
	for (int i = 0; i < gui_hierarchy_selection.size(); i++)
	{

	}

	//hierarchy_element > gui_hierarchy_selection
}

//--------------------------------------------------------------
void ofApp::updateHierarchySelection(bool& checked) {
	//Reference to toggle
	ofxToggle* toggle = nullptr;
		
	//Loop through hierarchy
	for (int i = 0; i < gui_hierarchy.getNumControls(); i++) {
		//Reference to toggle boxe
		ofxBaseGui* control = gui_hierarchy.getControl(i);
		toggle = dynamic_cast<ofxToggle*>(control);
		
		//If the toggle box is checked, make sure it's in the selection list
		if (toggle && toggle->value) {
			//Variable to check presence of element in selection
			bool found = false;

			for (size_t i = 0; i < gui_hierarchy_selection.size(); i++)
			{
				if (toggle->getName() == gui_hierarchy_selection[i].name) {
					found = true;
					std::cout << toggle->getName() << " was FOUND in gui selection : " << checked << std::endl;
					break;
				}
			}
											
			//std::cout << "Toggle '" << toggle->getName() << "' was toggled: " << checked << std::endl;
			
		}
	}
}

//--------------------------------------------------------------
void ofApp::camPosSliderXChanged(float& value){
	glm::vec3 p = cam_default->getPosition();
	//cam_default->setPosition(glm::vec3(value, p.y, p.z));
}

//--------------------------------------------------------------
void ofApp::camPosSliderYChanged(float& value) {
	glm::vec3 p = cam_default->getPosition();
	//cam_default->setPosition(glm::vec3(p.x, value, p.z));
}

//--------------------------------------------------------------
void ofApp::camPosSliderZChanged(float& value) {
	glm::vec3 p = cam_default->getPosition();
	cam_default->setPosition(glm::vec3(p.x, p.y, value));
	//cam_default->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

//--------------------------------------------------------------
void ofApp::camRotSliderChanged(glm::vec3& value){

}

//--------------------------------------------------------------
void ofApp::camFovSliderChanged(float& value){
	cam_default->setFov(value);
	//cam_default->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

//--------------------------------------------------------------
void ofApp::camClipNearSliderChanged(float& value){
	cam_default->setNearClip(value);
	//cam_default->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

//--------------------------------------------------------------
void ofApp::camClipFarSliderChanged(float& value){
	cam_default->setFarClip(value);
	//cam_default->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

//--------------------------------------------------------------
void ofApp::camSetDolly(float& value) {
	cam_dolly_speed = value;
	//cam_default->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

//--------------------------------------------------------------
void ofApp::camSetZoom(float& value) {
	cam_zoom_speed = value;
	//cam_default->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

//--------------------------------------------------------------
void ofApp::SunColor_AmbRValueChanged(float& value)
{
	//Get normalized value
	float _r = value / 255;
	cout << "Slider value changed: " << value << " representing " << _r * 100 << "%" << endl;

	sun_color_amb.r = value;
	sun_menu_color_amb.getFloatSlider("x").setFillColor(ofColor(value, 0, 0, 255));
	update();
}

//--------------------------------------------------------------
void ofApp::SunColor_AmbGValueChanged(float& value)
{
	//Get normalized value
	float _g = value / 255;
	cout << "Slider value changed: " << value << " representing " << _g * 100 << "%" << endl;

	sun_color_amb.g = value;
	sun_menu_color_amb.getFloatSlider("y").setFillColor(ofColor(0, value, 0, 255));
	update();
}

//--------------------------------------------------------------
void ofApp::SunColor_AmbBValueChanged(float& value)
{
	//Get normalized value
	float _b = value / 255;	
	cout << "Slider value changed: " << value << " representing " << _b * 100 << "%" << endl;	
	sun_color_amb.b = value;
	sun_menu_color_amb.getFloatSlider("z").setFillColor(ofColor(0, 0, value, 255));
	update();
}

//--------------------------------------------------------------
void ofApp::SunColor_DiffRValueChanged(float& value)
{
	//Get normalized value
	float _r = value / 255;
	cout << "Slider value changed: " << value << " representing " << _r * 100 << "%" << endl;

	sun_color_diff.r = value;
	sun_menu_color_diff.getFloatSlider("x").setFillColor(ofColor(value, 0, 0, 255));
	update();
}

//--------------------------------------------------------------
void ofApp::SunColor_DiffGValueChanged(float& value)
{
	//Get normalized value
	float _g = value / 255;
	cout << "Slider value changed: " << value << " representing " << _g * 100 << "%" << endl;

	sun_color_diff.g = value;
	sun_menu_color_diff.getFloatSlider("y").setFillColor(ofColor(0, value, 0, 255));
	update();
}

//--------------------------------------------------------------
void ofApp::SunColor_DiffBValueChanged(float& value)
{
	//Get normalized value
	float _b = value / 255;
	cout << "Slider value changed: " << value << " representing " << _b * 100 << "%" << endl;
	sun_color_diff.b = value;
	sun_menu_color_diff.getFloatSlider("z").setFillColor(ofColor(0, 0, value, 255));
	update();
}

//--------------------------------------------------------------
void ofApp::SunIntensityValueChanged(float& value)
{	
	sun_intensity_value = value;
	sun_intensity.setFillColor(ofColor(value*255, value * 255, value * 255, 255));	
	cout << "Sun constant attenuation is: " << sun->getAttenuationConstant() << ". Sun Linear attenuation is: " << sun->getAttenuationLinear() << endl;
	update();
}

//--------------------------------------------------------------
void ofApp::CreateNewLayer()
{
	cout << "button New layer is: clicked"  << endl;

	//Make a struct for hierarchy panel
	layer_2D* l = new layer_2D();
	std::stringstream ss;

	

	std::string sService = ss.str();
	l->index = layers_list.size();
	ss << "Layer_" << layers_list.size();
	l->name = ss.str();

	//Make a new toggle button and add it to the hierarchy
	ofxToggle* t = new ofxToggle();
	gui_layers.add(t->setup(l->name, false));

	//Add a listener to the new toggle
	t->addListener(this, &ofApp::UpdateLayersSelection);

	//Make added element selected by default
	layers_list.push_back(*l);
	std::cout << l->name << " has been created " << endl;

	update();
}

//--------------------------------------------------------------
void ofApp::DeleteSelectedLayers()
{

	//Reference to toggle
	ofxToggle* toggle = nullptr;

	//Loop through hierarchy
	for (int i = 0; i < gui_layers.getNumControls(); i++) {
		//Reference to toggle boxe
		ofxBaseGui* control = gui_layers.getControl(i);
		toggle = dynamic_cast<ofxToggle*>(control);

		//If the toggle box remove from layers list
		if (toggle->value) {
			

			auto it = std::find_if(layers_list.begin(), layers_list.end(),
				[&](const layer_2D& s) { return s.name == toggle->getName(); });

			if (it != layers_list.end()) {
				layers_list.erase(it);
			}
		}		
	}

	//Resize layer list
	layers_list.shrink_to_fit();

	//Reset gui_layer panel
	gui_layers.clear();
	for (size_t i = 0; i < layers_list.size(); i++)
	{
		//Make a new layer toggle and add it to the gui panel
		ofxToggle* t = new ofxToggle();
		gui_layers.add(t->setup(layers_list[i].name, false));

		//Add a listener to the new toggle
		t->addListener(this, &ofApp::UpdateLayersSelection);
	}

	//Update app
	update();
}

//--------------------------------------------------------------
void ofApp::UpdateLayersSelection(bool& checked) {
	//Reference to toggle
	ofxToggle* toggle = nullptr;

	//Clear selection list
	selected_layers_list.clear();

	//Loop through layers list
	for (int i = 0; i < layers_list.size(); i++) {
		
		//Reference to toggle boxe
		ofxBaseGui* control = gui_layers.getControl(i);
		toggle = dynamic_cast<ofxToggle*>(control);

		//If the toggle box is checked, make sure it's in the selection list
		if (toggle && toggle->value) {
			
			selected_layers_list.push_back(&layers_list[i]);
			std::cout << toggle->getName() << " was FOUND in Layer selection : " << std::endl;

		}

		//Resize selection vector
		selected_layers_list.shrink_to_fit();
	}

	//std::cout << "selected layer contains: " << endl;
}