#include <PieceFactory.h>

Piece& PieceFactory::CreatePiece(Global::Piece_Type p_type, Global::Color color) {
    sf::Vector2f size = sf::Vector2f(75.0f, 75.0f);
    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
    Piece* p = new Piece();
    std::string colorString = Global::GetStringFromColor(color);
    if (colorString.empty())
        return *p;
    std::string resourceName = "/textures/" + colorString + "/";
    std::string textureFileName = Global::GetStringFromPieceType(p_type) + ".png";

    if (!texture->loadFromFile(RESOURCE + resourceName + textureFileName)) {
        std::cout << "Failed to load " << colorString << " " << textureFileName << " texture." << std::endl;
    }
    texture->setSmooth(true);
    
    p = new Piece(size,texture,p_type,color);
    return *p;
}
