#include "TileMap.h"

using namespace WallG;
using namespace WallG::Input;
using namespace WallG::Graphics;

namespace
{
    TileMap* sInstance = nullptr;
    Math::Vector2 GetTargetPostition()
    {
        int MouseX = Input::InputSystem::Get()->GetMouseScreenX();
        int MouseY = Input::InputSystem::Get()->GetMouseScreenY();
        return { static_cast<float>(MouseX), static_cast<float>(MouseY) };
    }
    inline int GetIndex(int x, int y, int columns)
    {
        return x + (y * columns);
    }

}


void TileMap::StaticInitialize()
{
    ASSERT(sInstance == nullptr, "EnemeyManager areadly intialized");
    sInstance = new TileMap();
}

void TileMap::StaticTerminate()
{
    if (sInstance)
    {
        delete sInstance;
        sInstance = nullptr;
    }
}

TileMap& TileMap::Get()
{
    // TODO: insert return statement here
    ASSERT(sInstance != nullptr, "No enemeyManager areadly intialized");
    return *sInstance;
}

void TileMap::LoadTiles(std::filesystem::path fileName)
{
    // TOFO: Load Tile based on the passed in file
    //mTiles.push_back(X::LoadTexture("grass.png"));
    //mTiles.push_back(X::LoadTexture("flower.png"));
    //mTiles.push_back(X::LoadTexture("stone.png"));
    //mTileWidth = X::GetSpriteWidth(mTiles.front());
    //mTileHeight = X::GetSpriteHeight(mTiles.front());

    FILE* file = nullptr;
    fopen_s(&file, fileName.u8string().c_str(), "r");

    int count = 0;
    fscanf_s(file, "Count: %d\n", &count);

    mTiles.resize(count);
    for (int i = 0; i < count; ++i)
    {
        char buffer[256];
        fscanf_s(file, "%s\n", buffer, static_cast<int>(std::size(buffer)));
        mTiles[i] = TextureManager::Get()->Load(buffer);
    }
    fclose(file);

    mTileWidth = SpriteRenderer::Get()->GetSpriteWidth(mTiles.front());
    mTileHeight = SpriteRenderer::Get()->GetSpriteHeight(mTiles.front());
}

void TileMap::LoadMap(std::filesystem::path fileName)
{
    if (!std::filesystem::exists(fileName))
        return;

    std::ifstream file(fileName);

    int columns = 0, rows = 0;
    std::string buffer;

    file >> buffer;
    file >> columns;
    file >> buffer;
    file >> rows;

    //Resize(columns, rows);
    mMap.resize(columns * rows);
    mColumns = columns;
    mRows = rows;


    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < columns; ++x)
        {
            char c;
            file >> c;
            mMap[GetIndex(x, y, columns)] = c - '0';
        }
    }

    file.close();
}

void TileMap::Resize(int columns, int rows)
{
     //TODO: implement this
    std::vector<int> temp;
    temp.resize(columns * rows);
    int textureID = 0;
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < columns; ++x)
        {
            if (x < mColumns && y < mRows)
            {
                textureID = mMap[GetIndex(x, y, mColumns)];
            }
            else
            {
                textureID = 0;
            }
            temp[GetIndex(x, y, columns)] = textureID;
        }
    }

    mMap.clear();
    mColumns = columns;
    mRows = rows;
    mMap.resize(mColumns * mRows);
    for (int y = 0; y < mRows; ++y)
    {
        for (int x = 0; x < mColumns; ++x)
        {
            int i = GetIndex(x, y, mColumns);
            mMap[i] = temp[i];
        }
    }
}

void TileMap::Update()
{
    auto inputSystem = InputSystem::Get();
    if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
    {
        int mouseX = inputSystem->GetMouseScreenX();
        int mouseY = inputSystem->GetMouseScreenY();
        int column = mouseX / mTileWidth;
        int row = mouseY / mTileHeight;
        //TODO:Check bound to makesure we are click inside the map
        if (mTextureId <3)
        {
            mMap[GetIndex(column, row, mColumns)] = mTextureId;
        }
        else
        {
            mMap[GetIndex(column, row, mColumns)] = 3;
            mMap[GetIndex(column+1, row, mColumns)] = 4;
            mMap[GetIndex(column, row+1, mColumns)] = 5;
            mMap[GetIndex(column+1, row+1, mColumns)] = 6;
        }
    }
}

void TileMap::SaveMap(std::filesystem::path fileName)
{
    std::ofstream outfile(fileName);
    outfile << "Columns: " << mColumns << std::endl;
    outfile << "Rows: " << mRows << std::endl;

    for (int y = 0; y < mRows; ++y)
    {
        for (int x = 0; x < mColumns; ++x)
        {
            int i = GetIndex(x, y, mColumns);
            outfile << mMap[i];
        }
        outfile << std::endl;
    }

    outfile.close();
}

void TileMap::changeTile()
{
    auto inputSystem = InputSystem::Get();
    if (inputSystem->IsKeyPressed(KeyCode::ONE))
    {
        mTextureId = 0;
    }
    if (inputSystem->IsKeyPressed(KeyCode::TWO))
    {
        mTextureId = 1;
    }
    if (inputSystem->IsKeyPressed(KeyCode::THREE))
    {
        mTextureId = 2;
    }
    if (inputSystem->IsKeyPressed(KeyCode::FOUR))
    {
        mTextureId = 3;
    }
    if (inputSystem->IsKeyPressed(KeyCode::FIVE))
    {
        mTextureId = 4;
    }
    if (inputSystem->IsKeyPressed(KeyCode::SIX))
    {
        mTextureId = 5;
    }
}

void TileMap::Render() const
{
    int x = 0, y = 0, index = 0;
    Math::Vector2 pos;
    for (y = 0, pos.y = 0.0f; y < mRows; ++y, pos.y += mTileHeight)
        for (x = 0, pos.x = 0.0f; x < mColumns; ++x, pos.x += mTileWidth)
            SpriteRenderer::Get()->DrawSprite(mTiles[mMap[index++]], pos, 0.0f, Pivot::TopLeft);
}

void TileMap::ShowDebugUI()
{
    if (ImGui::CollapsingHeader("Map Editor", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("Load Map"))
        {
            char fileName[MAX_PATH] = {};

            LoadMap(fileName);
        }
        if (ImGui::Button("Save Map"))
        {
            char fileName[MAX_PATH] = {};

            SaveMap(fileName);
        }
        ImGui::Separator();

        static int columns = mColumns;
        static int rows = mRows;
        ImGui::SliderInt("Columns", &columns, 5, 50);
        ImGui::SliderInt("Rows", &rows, 5, 50);
        if (ImGui::Button("Resize Map"))
        {
            Resize(columns, rows);
        }

        for (int i = 0; i < (int)mTiles.size(); ++i)
        {
            if (ImGui::ImageButton(SpriteRenderer::Get()->GetSprite(mTiles[i]), { (float)mTileWidth, (float)mTileHeight }))
                mTextureId = i;
            ImGui::SameLine();
        }
        ImGui::Separator();
    }
}

float TileMap::traversalCost(int fromX, int fromY, int toX, int toY)
{
    int fromTile = mMap[GetIndex(fromX, fromY, mColumns)];
    int toTile = mMap[GetIndex(toX, toY, mColumns)];
    float cost = 1.0f;
    if (toTile == 1)
    {
        cost = 2.0f;
    }

    if (fromX != toX && toY != fromY)
    {
        cost *= 1.4142f;
    }
    return cost;
}

float TileMap::ManhattanDistence(int fromX, int fromY, int toX, int toY)
{
    float D = 1;
    float dx = abs(fromX - toX);
    float dy = abs(fromY - toY);
    return D * (dx + dy);
}

float TileMap::Euclidean(int fromX, int fromY, int toX, int toY)
{
    float D = 1;
    float dx = abs(fromX - toX);
    float dy = abs(fromY - toY);
    return  D * sqrt(dx * dx + dy * dy);
}

float TileMap::Diagonal(int fromX, int fromY, int toX, int toY)
{
    float D = 1;
    float D2 = sqrt(2);
    float dx = abs(fromX - toX);
    float dy = abs(fromY - toY);
    return D * Math::Max(dx, dy) + (D2 - D) * Math::Min(dx, dy);
}

bool TileMap::IsBlocked(int columns, int rows) const
{
    //TODO: DO bound check!
    //TODO: properly support what is a blocked tile when we load different image
    return mMap[GetIndex( columns, rows, mColumns)] >= 2;
}


