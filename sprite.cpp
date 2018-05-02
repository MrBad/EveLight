#include "sprite.h"

Sprite::Sprite(float x, float y, float width, float height, Texture *texture, GLProgram &program) : FilledRectangle(x, y, width, height)
{
    mTexture = texture;
    mProgram = &program;
    cout << "sprite: " << mTexture->getName() << endl;
}

void Sprite::Draw()
{
    // bind texture //
    glUseProgram(mProgram->getId());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture->getId());
    GLint textureLocation = glGetUniformLocation(mProgram->getId(), "texSampler");
    glUniform1i(textureLocation, 0);
    
    Mesh::Draw();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}