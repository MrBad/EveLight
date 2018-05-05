#include "sprite.h"

Sprite::Sprite(float x, float y, float width, float height, const Color &color, uint texId)
    : FilledRectangle(x, y, width, height, color)
{
    textureId = texId;
}

void Sprite::Draw()
{
    // // bind texture //
    // glUseProgram(mProgram->getId());
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, mTexture->getId());
    // GLint textureLocation = glGetUniformLocation(mProgram->getId(), "texSampler");
    // glUniform1i(textureLocation, 0);

    // //Mesh::Draw();

    // glBindTexture(GL_TEXTURE_2D, 0);
    // glDisable(GL_TEXTURE_2D);
}
