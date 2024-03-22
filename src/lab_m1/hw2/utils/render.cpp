#include "render.h"

#include "lab_m1/hw2/objects/building.h"

void custom::RenderMesh(
    Mesh *mesh, custom::Material *material, custom::Camera *camera, Shader *shader, const glm::mat4 &modelMatrix,
    const glm::vec3 &lightPosition, const float health
    ) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    int location = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(location, 1, glm::value_ptr(lightPosition));

    glm::vec3 eyePosition = camera->position;
    location = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(location, 1, glm::value_ptr(eyePosition));

    location = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(location, material->GetShininess());

    location = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(location, material->GetKd());

    location = glGetUniformLocation(shader->program, "materil_ks");
    glUniform1f(location, material->GetKs());

    location = glGetUniformLocation(shader->program, "material_ka");
    glUniform1f(location, material->GetKa());

    location = glGetUniformLocation(shader->program, "material_ke");
    glUniform1f(location, material->GetKe());

    location = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(location, 1, glm::value_ptr(material->GetColor()));

    location = glGetUniformLocation(shader->program, "health_in");
    glUniform1f(location, health);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}
