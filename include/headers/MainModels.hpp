#include "Model.hpp"

std::map<int, Model> CurrentModels;

void SetupModels(std::string file = "res/models/models.json")
{
	std::string text = get_file_contents( (file).c_str() );
    json JSONModels = json::parse(text);

	for(int c = 0; c <= JSONModels["Models"].size() - 1; c++)
    {
        json Position = JSONModels["Models"][c]["Position"][0];
		json Rotation = JSONModels["Models"][c]["Rotation"][0];
		json Size = JSONModels["Models"][c]["Size"][0];


		std::string GroundPath = JSONModels["Models"][c]["Path"];

        Model model = {(GroundPath).c_str(),
		glm::vec3(Position["x"], Position["y"], Position["z"]),
		glm::quat(Rotation["x"], Rotation["y"], Rotation["z"], Rotation["w"]),
		glm::vec3(Size["x"], Size["y"], Size["z"]),
		JSONModels["Models"][c]["TexturePath"]};
		
        model.IsNPC = JSONModels["Models"][c]["NPC"];

        CurrentModels.insert(std::pair<int, Model>(c, model));
    }
}

void ClearModels()
{
	CurrentModels.clear();
}

std::map<int, Model> GetModels()
{
	return CurrentModels;
}

void SetModel(int modelID, Model value)
{
	CurrentModels[modelID] = value;
}