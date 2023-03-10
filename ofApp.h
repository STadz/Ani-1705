#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	public:
		struct hierarchy_element {
			int index;
			string name;
			bool selected = false;
		};

		struct element_2D {
			ofColor col;
			bool outlined;
			bool filled;
			vector<element_2D*> layers_drawn;
		};

		struct layer_2D {
			int index;
			string name;
			float alpha = 1.0f;			
		};
	
	//Custom app methods
	private:		
		void setBgColor();
		void setKeyModifier();
		void FloatSliderValueChanged(float& value);
		void resetGUI();
		void deleteHierarchySelection();
		void updateHierarchySelection(bool& value);
		
		//Camera
		void camPosSliderXChanged(float& value);
		void camPosSliderYChanged(float& value);
		void camPosSliderZChanged(float& value);
		void camRotSliderChanged(glm::vec3& value);
		void camFovSliderChanged(float& value);
		void camClipNearSliderChanged(float& value);
		void camClipFarSliderChanged(float& value);
		void camSetDolly(float& value);
		void camSetZoom(float& value);

		//Light
		void SunColor_AmbRValueChanged(float& value);
		void SunColor_AmbGValueChanged(float& value);
		void SunColor_AmbBValueChanged(float& value);
		void SunColor_DiffRValueChanged(float& value);
		void SunColor_DiffGValueChanged(float& value);
		void SunColor_DiffBValueChanged(float& value);
		void SunIntensityValueChanged(float& value);
		
		//Layers
		void CreateNewLayer();
		void DeleteSelectedLayers();
		void UpdateLayersSelection(bool& checked);

	private:
		enum GizmoMode { translate, rotate, scale };
		enum BackgroundMode { solid, image, skybox};


		GizmoMode gizmo_mode = GizmoMode::translate;
		BackgroundMode background_mode = BackgroundMode::solid;

		//Key modifiers
		bool key_ctrl_held;
		bool key_alt_held;
		bool key_shift_held;
		bool key_modifier_any;	

		//Mouse
		bool mouse_wheel_held;
		bool mouse_left_held;
		bool mouse_right_held;
		bool mouse_any_held;
		float zoom_scroll_speed = 5.0f;
		ofVec2f mouse_click_pos;
		ofVec2f mouse_rel_pos;

		//2D drawing
		bool is_gui_shown = true;
		bool onion_skin   = false;

		std::vector<ofMesh>		meshes_list;			//Todo
		std::vector<ofShader>	shader_list_list;		//Todo
		std::vector<ofTexture>	textures_list;			//Todo
		std::vector<ofImage>	images_list;			//Todo
		std::vector<hierarchy_element> gui_hierarchy_selection;
		std::vector<ofCamera>	cameras_list;			//Todo
		std::vector<ofLight>	lights_list;			//Todo
		std::vector<layer_2D>	layers_list;
		std::vector<layer_2D*>	selected_layers_list;

		ofImageLoadSettings		image_settings;	
		ofTexture				image_texture;

		//Default shader
		ofShader	shader_default;
		
		//Lights
		ofLight* sun = new ofLight();									//Todo	
		ofColor	sun_color_diff;
		ofColor	sun_color_amb;
		ofxVec3Slider  sun_menu_color_amb;
		ofxVec3Slider  sun_menu_color_diff;
		ofxFloatSlider sun_intensity;
		float sun_intensity_value;

		

		//Background
		ofColor background_color = ofColor::aliceBlue;	

		//Camera
		ofCamera*	 cam_default = new ofCamera();							//Todo
		ofVec3f		 cam_position;
		ofVec3f		 cam_target;
		ofQuaternion cam_orientation;
		float		 cam_clip_n;
		float		 cam_clip_f;
		float		 cam_fov;
		float		 cam_dolly_speed = 0.00012f;
		float		 cam_zoom_speed  = -0.00012f;

		//Mouse
		ofVec2f mouse_delta;

		//Default meshes
		ofxAssimpModelLoader character_model;
		std::vector<ofxAssimpModelLoader> bg_models;

		

		//GUI
		ofColor menu_color_border = ofColor::darkSlateBlue;
		ofColor menu_color_bg = ofColor::darkBlue;
		ofColor menu_color_headerbg = ofColor::mediumSlateBlue;

		ofTrueTypeFont gui_font;

		std::vector<ofxPanel*>    gui_menu_list;
		
		ofxPanel gui_file_menu;
		ofxPanel gui_3D_menu;
		ofxPanel gui_image_menu;
		ofxPanel gui_camera_menu;
		ofxPanel gui_hierarchy;
		ofxPanel gui_light_menu;
		ofxPanel gui_layers;

		
		//Camera menu
		ofxVec3Slider  cam_menu_pos;
		ofxVec3Slider  cam_menu_rot;
		ofxFloatSlider cam_menu_fov;
		ofxFloatSlider cam_menu_clip_near;
		ofxFloatSlider cam_menu_clip_far;
		ofxFloatSlider cam_menu_dolly_speed;
		ofxFloatSlider cam_menu_zoom_speed;

		//Layers menu
		ofxButton layer_new;
		ofxButton layer_del;

		//Examples/tests
		ofxIntSlider   intSlider;
		ofxFloatSlider floatSlider;
		ofxToggle toggle;
		ofxButton button;
		ofxLabel label;
		ofxIntField intField;
		ofxFloatField floatField;
		ofxTextField textField;
		ofxVec2Slider vec2Slider;
		ofxVec3Slider vec3Slider;
		ofxVec4Slider vec4Slider;		


		//Supported image file formats
		std::string JPG  = ".jpg";
		std::string JPEG = ".jpeg";
		std::string PNG  = ".png";
		std::string BMP  = ".bmp";

		//Supported 3D file formats
		std::string OBJ = ".obj";
		std::string FBX = ".fbx";
		std::string CAD = ".cad";
};
