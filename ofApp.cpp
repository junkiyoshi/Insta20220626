#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
	glm::vec3 noise_param = glm::vec3(ofRandom(360), ofRandom(360), ofRandom(360));

	if (ofGetFrameNum() % 2 == 0) {

		this->radius_list.push_back(0);
		//this->rotation_list.push_back(glm::vec3(ofRandom(360), ofRandom(360), ofRandom(360)));

		this->rotation_list.push_back(glm::vec3(

			ofMap(ofNoise(noise_param.x, ofGetFrameNum() * 0.006), 0, 1, -360, 360),
			ofMap(ofNoise(noise_param.y, ofGetFrameNum() * 0.006), 0, 1, -360, 360),
			ofMap(ofNoise(noise_param.z, ofGetFrameNum() * 0.006), 0, 1, -360, 360)));
	}

	for (int i = this->radius_list.size() - 1; i > -1; i--) {

		this->radius_list[i] += 5;

		if (this->radius_list[i] > 250) {

			this->radius_list.erase(this->radius_list.begin() + i);
			this->rotation_list.erase(this->rotation_list.begin() + i);
		}
	}

	this->face.clear();
	this->frame.clear();

	ofColor color;
	for (int i = 0; i < this->radius_list.size(); i++) {

		color.setHsb((i * 30) % 255, 200, 255);

		auto alpha = this->radius_list[i] < 30 ? 0 : ofMap(this->radius_list[i], 30, 250, 0, 255);
		this->setRingToMesh(this->face, this->frame, glm::vec3(), this->rotation_list[i], this->radius_list[i], this->radius_list[i] * 0.1, ofColor(color, alpha), ofColor(255, alpha));
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 1.33333333333);

	this->face.draw();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, glm::vec3 rotation, float radius, float height, ofColor face_color, ofColor frame_color) {

	int index = face_target.getNumVertices();

	for (int deg = 0; deg < 360; deg += 5) {

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 5) * DEG_TO_RAD), radius * sin((deg + 5) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 5) * DEG_TO_RAD), radius * sin((deg + 5) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			auto rotation_x = glm::rotate(glm::mat4(), rotation.x * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), rotation.y * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), rotation.z * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));

			vertex = glm::vec4(vertex, 0) * rotation_y * rotation_x + glm::vec4(location, 0);
		}

		auto face_index = face_target.getNumVertices();
		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		auto frame_index = frame_target.getNumVertices();
		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
			frame_target.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}