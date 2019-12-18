////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#undef SFML_OPENGL_ES
#define SFML_OPENGL_ES2

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include "sokol.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
extern "C"
{
	void keydown(int key_code);
	void keyup(int key_code);
	int getTapePos();
}
////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Request a 24-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(0, 0), "SFML graphics with OpenGL-ES2", sf::Style::Default, contextSettings); 

    // Make it the active window for OpenGL calls
    window.setActive();	
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontFromFileTTF("namyangju_godic.otf", 16.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	io.MouseDrawCursor = true;
	io.DisplaySize = ImVec2((float)window.getSize().x, (float)window.getSize().y);	
	ImGui_ImplOpenGL3_Init(0); 	
	ImGui_ImplOpenGL3_NewFrame();

	sapp_desc desc = sokol_main(argc, argv);
	desc.width = io.DisplaySize.x;
	desc.height = io.DisplaySize.y;
    _sapp_init_state(&desc);
	_sapp.valid = true;
	_sapp.frame_count = 0;	
	_sapp_call_init();
	int w;
    while (window.isOpen())
    {
		io.MousePos = ImVec2((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y);
		io.MouseDown[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		io.MouseDown[1] = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		io.MouseDown[2] = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
		
        // Handle other events
        sf::Event event;
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
			{
				int c = event.key.code;
				switch (event.key.code)
				{
					case sf::Keyboard::Space:		 c = 0x20; break;
					case sf::Keyboard::Left:		 c = 0x80; break;
					case sf::Keyboard::Right:        c = 0x09; break;
					case sf::Keyboard::Down:         c = 0x0A; break;
					case sf::Keyboard::Up:           c = 0x0B; break;
					case sf::Keyboard::Return:       c = 0x0D; break;
					case sf::Keyboard::BackSpace:    c = 0x0C; break;
					case sf::Keyboard::Escape:       c = 0x07; break;
					case sf::Keyboard::RControl: 
					case sf::Keyboard::LControl: 	 c = 0x0F; break; 
					case sf::Keyboard::F1:           c = 0xF1; break;
					case sf::Keyboard::F2:           c = 0xF2; break;
					case sf::Keyboard::F3:           c = 0xF3; break;
					case sf::Keyboard::F4:           c = 0xF4; break;
					case sf::Keyboard::F5:           c = 0xF5; break;
					case sf::Keyboard::End:          c = 0xF6; break;
					case sf::Keyboard::Capslock:	 c = 0xF7; break;
					case sf::Keyboard::RAlt:
					case sf::Keyboard::LAlt:		 c = 0xF8; break;
					case sf::Keyboard::RShift:
					case sf::Keyboard::LShift:		 c = 0x0E; break;
					case sf::Keyboard::LBracket:	 c = '[';  break;
					case sf::Keyboard::RBracket:	 c = ']';  break;
					case sf::Keyboard::Comma:	 	 c = ',';  break;
					case sf::Keyboard::Period:	 	 c = '.';  break;
					case sf::Keyboard::Quote:	 	 c = '\''; break;
					case sf::Keyboard::Slash:	 	 c = '/';  break;
					case sf::Keyboard::BackSlash:	 c = '\\'; break;
					case sf::Keyboard::Tilde:	 	 c = '~';  break;
					case sf::Keyboard::Equal:	 	 c = '=';  break;
					case sf::Keyboard::Dash:	 	 c = '-';  break;
					case sf::Keyboard::SemiColon:	 c = ';';  break;
					case sf::Keyboard::Delete:	     c = 0xF6; break;
					case sf::Keyboard::F12:			 exit(0);
					default:
						if (c >= sf::Keyboard::Num0 && c <= sf::Keyboard::Num9)
							c = c - sf::Keyboard::Num0 + '0'; 
						else if (c >= sf::Keyboard::A && c <= sf::Keyboard::Z)
							c = c - sf::Keyboard::A + 'A';
						else
						{
							switch (event.text.unicode)
							{
							case '!':				
							case '@':
							case '#':
							case '$':
							case '^':
							case '&':
							case '*':
							case '(':
							case ')':
							case '+':
							case ':':
							case '_':
							c = event.text.unicode; break;
							default:
							if (event.text.unicode >= 'a' && event.text.unicode <= 'z')
								c = event.text.unicode;
						}
					}
				}
				if (event.type == sf::Event::KeyPressed)
					keydown(c);
				else
					keyup(c);
			}			
		}		
		ImGui::NewFrame();
		if ((w = getTapePos())>0)
		{
			bool g_bMenuOpen = false;
			ImVec2 xy(0,0);
			ImGui::Begin("_", &g_bMenuOpen, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysUseWindowPadding);
			ImGui::SetNextWindowPos(xy);
			ImGui::SetWindowSize(ImVec2((float)io.DisplaySize.x, 40.0f));
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowBorderSize = 0.0f;  
			ImGui::SetWindowFocus("top"); 
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0,23), ImVec2((float) (io.DisplaySize.x), 40), IM_COL32(87,50,50,255));
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(w * io.DisplaySize.x / 100.0f - 10,23), ImVec2((float) (w * io.DisplaySize.x / 100.0f), 40), IM_COL32(0,180,81,255));//ImVec2(10,10), ImVec2(320,20), IM_COL32(0,255,255,55));
			ImGui::SetWindowPos(ImVec2(2,15));
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Tape Loading: %3d%%", (int)w);
			ImGui::End();			
		}
		ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
		_sapp_call_frame();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());		
		_sapp.frame_count++;
		window.display();
    }
	return 0;
#if 0	
    // Our state
    bool show_demo_window = true;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// Main loop
    while (window.isOpen())
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		// Handle mouse events
		io.MousePos = ImVec2((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y);
		io.MouseDown[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		io.MouseDown[1] = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		io.MouseDown[2] = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
		
        // Handle other events
        sf::Event event;
        while (window.pollEvent(event))
        {

		}			
        // Start the Dear ImGui frame
		ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
	
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
			static int counter = 0;

            ImGui::Begin("Hello, World!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
			ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("counter = %d, %d, %d", counter, (int)io.MousePos.x, (int)io.MousePos.y);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        //Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		window.display();
    }
	return 0;
#endif
}
