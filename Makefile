.PHONY: clean All

All:
	@echo "----------Building project:[ systick_irq - Debug ]----------"
	@cd "systick_irq" && "$(MAKE)" -f  "systick_irq.mk" && "$(MAKE)" -f  "systick_irq.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ systick_irq - Debug ]----------"
	@cd "systick_irq" && "$(MAKE)" -f  "systick_irq.mk" clean
