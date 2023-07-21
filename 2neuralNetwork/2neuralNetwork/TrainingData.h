#pragma once
#include <vector>
#include <string>

struct DataStructure {

	std::vector<float> inputs;
	std::vector<float> targets;
};

class TrainingData {
public:
	static std::vector<DataStructure> XorData() {
		
		std::vector<DataStructure> data;

		DataStructure tt;
		tt.inputs = { 1.0f, 1.0f };
		tt.targets = { 0.0f };
		data.push_back(tt);

		DataStructure tf;
		tf.inputs = { 1.0f, 0.0f };
		tf.targets = { 1.0f };
		data.push_back(tf);

		DataStructure ft;
		ft.inputs = { 0.0f, 1.0f };
		ft.targets = { 1.0f };
		data.push_back(ft);

		DataStructure ff;
		ff.inputs = { 0.0f, 0.0f };
		ff.targets = { 0.0f };
		data.push_back(ff);

		return data;
	}

	static std::vector<DataStructure> AndData() {

		std::vector<DataStructure> data;

		DataStructure tt;
		tt.inputs = { 1.0f, 1.0f };
		tt.targets = { 1.0f };
		data.push_back(tt);

		DataStructure tf;
		tf.inputs = { 1.0f, 0.0f };
		tf.targets = { 0.0f };
		data.push_back(tf);

		DataStructure ft;
		ft.inputs = { 0.0f, 1.0f };
		ft.targets = { 0.0f };
		data.push_back(ft);

		DataStructure ff;
		ff.inputs = { 0.0f, 0.0f };
		ff.targets = { 0.0f };
		data.push_back(ff);

		return data;
	}

};

